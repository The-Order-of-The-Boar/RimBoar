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
#include "../pathfinding/pathfinder.hpp"
#include "../utils/logging/assert.hpp"
#include "../utils/logging/log.hpp"
#include "../utils/print_utils.hpp"
#include "../utils/time_utils.hpp"
#include "scene.hpp"
#include "../audio/audio_manager.hpp"


Map create_map(glm::u32vec2 size)
{
    auto output = Map{size, [&](glm::u32vec2) { return Tile{}; }};
    return output;
}

GameScene::GameScene(AudioManager* audio_manager) : Scene{audio_manager}
{
    this->audio_manager->play_music("game.wav");
    this->world = std::unique_ptr<World>{new World{.map = create_map({10, 10})}};

    this->world->push_unit({}, {1, 1});

    for (size_t y = 1; y < 9; ++y)
        this->world->push_wall({}, {3, y});
}

GameScene::~GameScene() {}

void GameScene::update(double const delta)
{
    (void)delta;
    auto update_test_path = [this]()
    {
        TimeMeasurer pathfinding_time{"Found path in"};

        if (auto path = this->world->map.pathfinder->get_path(this->world->map.test_entity_index,
                                                              this->world->map.test_target);
            path.has_value())
            this->world->map.test_path = path.value();
        else
            this->world->map.test_path = {};

        pathfinding_time.print_time();
    };
    auto update_graph_and_path = [this, update_test_path]()
    {
        this->world->map.update_graph_representation();
        update_test_path();
    };

    // Construction Mode
    if (this->current_interaction_mode == CONSTRUCTION)
    {
        if (this->left_mouse_pressed && this->world->map.is_inside_boundaries(this->hovered_index))
        {
            Tile& hovered_tile = this->world->map.get(this->hovered_index.x, this->hovered_index.y);
            if (hovered_tile.is_occupied() == false)
            {
                world->push_wall(Wall{}, this->hovered_index);
                update_graph_and_path();
            }
        }
        else if (this->right_mouse_pressed &&
                 this->world->map.is_inside_boundaries(this->hovered_index))
        {
            Tile& hovered_tile = this->world->map.get(this->hovered_index.x, this->hovered_index.y);
            if (hovered_tile.wall.has_value())
            {
                this->world->remove_wall(this->hovered_index);
                update_graph_and_path();
            }
        }
    }

    // Command Mode
    if (this->current_interaction_mode == COMMAND)
    {
        if (this->left_mouse_pressed && this->world->map.is_inside_boundaries(this->hovered_index))
        {
            Tile& hovered_tile = this->world->map.get(this->hovered_index.x, this->hovered_index.y);
            if (hovered_tile.is_occupied() == false)
            {
                this->world->remove_unit(this->world->map.test_entity_index);
                this->world->push_unit(Unit{}, this->hovered_index);
                this->world->map.test_entity_index = hovered_index;

                update_graph_and_path();
            }
        }
        else if (this->right_mouse_pressed &&
                 this->world->map.is_inside_boundaries(this->hovered_index))
        {
            Tile& hovered_tile = this->world->map.get(this->hovered_index.x, this->hovered_index.y);
            if (hovered_tile.is_occupied() == false &&
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
    // ImGui::ShowDemoWindow();

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