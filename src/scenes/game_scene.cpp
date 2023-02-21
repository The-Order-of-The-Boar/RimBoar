#include "./game_scene.hpp"

// builtin
#include <ostream>
#include <vector>

// extern
#include <SDL.h>
#include <SDL_stdinc.h>
#include <glm/vec2.hpp>
#include <imgui.h>

// local
#include "../core/world.hpp"
#include "../logging/assert.hpp"
#include "../logging/log.hpp"
#include "../pathfinding/pathfinder.hpp"
#include "../utils/print_utils.hpp"
#include "../utils/time_utils.hpp"
#include "scene.hpp"


Map create_map(glm::u32vec2 size)
{
    auto output = Map{size, [&](glm::u32vec2) { return Tile{.state = Tile::State::Emtpy}; }};
    return output;
}

GameScene::GameScene()
{
    this->world = std::unique_ptr<World>{new World{.map = create_map({20, 10})}};
    this->world->map.get(this->world->map.test_entity_index).state = Tile::State::Occupied;
}

GameScene::~GameScene() {}

void GameScene::update(double const delta)
{
    (void)delta;
    auto update_test_path = [this]()
    {
        TimeMeasurer pathfinding_time{"Found path in"};
        this->world->map.test_path = this->world->map.pathfinder->get_path(
            this->world->map.test_entity_index, this->world->map.test_target);
        // pathfinding_time.print_time();
    };
    auto update_tile_state = [this, update_test_path](Tile& tile, const Tile::State new_state)
    {
        tile.state = new_state;
        this->world->map.update_graph_representation();
        update_test_path();
    };

    // Construction Mode
    if (this->current_interaction_mode == CONSTRUCTION)
    {
        if (this->left_mouse_pressed && this->world->map.is_inside_boundaries(this->hovered_index))
        {
            Tile& hovered_tile = this->world->map.get(this->hovered_index.x, this->hovered_index.y);
            if (hovered_tile.state == Tile::State::Emtpy)
                update_tile_state(hovered_tile, Tile::State::Wall);
        }
        else if (this->right_mouse_pressed &&
                 this->world->map.is_inside_boundaries(this->hovered_index))
        {
            Tile& hovered_tile = this->world->map.get(this->hovered_index.x, this->hovered_index.y);
            if (hovered_tile.state == Tile::State::Wall)
                update_tile_state(hovered_tile, Tile::State::Emtpy);
        }
    }

    // Command Mode
    if (this->current_interaction_mode == COMMAND)
    {
        if (this->left_mouse_pressed && this->world->map.is_inside_boundaries(this->hovered_index))
        {
            Tile& hovered_tile = this->world->map.get(this->hovered_index.x, this->hovered_index.y);
            if (hovered_tile.state == Tile::State::Emtpy)
            {
                this->world->map.get(this->world->map.test_entity_index).state = Tile::State::Emtpy;
                this->world->map.test_entity_index = hovered_index;
                hovered_tile.state = Tile::State::Occupied;
                this->world->map.update_graph_representation();
                update_test_path();
            }
        }
        else if (this->right_mouse_pressed &&
                 this->world->map.is_inside_boundaries(this->hovered_index))
        {
            Tile& hovered_tile = this->world->map.get(this->hovered_index.x, this->hovered_index.y);
            if (hovered_tile.state == Tile::State::Emtpy &&
                this->hovered_index != this->world->map.test_target)
            {
                this->world->map.test_target = this->hovered_index;
                update_test_path();
            }
        }
    }
}

void GameScene::render(SDL_Renderer* renderer) const
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    this->renderer.render(renderer, *this->world);
}

void GameScene::update_hud()
{
    // notice("game hud");
    bool panel_visible = true;
    ImGui::Begin("Game", &panel_visible);
    ImGui::Text("Be welcome to our game world!");
    ImGui::ShowDemoWindow();

    auto const io = ImGui::GetIO();
    this->left_mouse_pressed = io.MouseDown[0];
    this->right_mouse_pressed = io.MouseDown[1];
    this->hovered_index = renderer.screen_to_index(glm::i32vec2{io.MousePos.x, io.MousePos.y});

    if (ImGui::Button("Main Menu"))
    {
        this->scene_status = {true, MENU};
    }
    else if (ImGui::Button("Quit"))
    {
        this->scene_status = {true, QUIT};
    }

    ImGui::RadioButton("Command Mode", &this->current_interaction_mode, COMMAND);
    ImGui::RadioButton("Construction Mode", &this->current_interaction_mode, CONSTRUCTION);

    ImGui::End();
}