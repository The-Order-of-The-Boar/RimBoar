#include "./game_scene.hpp"

// third party
#include <SDL.h>
#include <glm/ext/vector_int2_sized.hpp>
#include <imgui.h>

// local
#include "../logging/log.hpp"
#include "scene.hpp"
#include "../pathfinding/pathfinder.hpp"
#include "../utils/print_utils.hpp"
#include "../utils/time_utils.hpp"

GameScene::GameScene()
{
    TempWorld game_world{};
    Pathfinder pathfinder{&game_world.connection_graph, game_world.world_size};

    glm::i32vec2 start{00,0};
    glm::i32vec2 end{99,99};

    TimeMeasurer path_timer{"Found path in"};
    const auto path = pathfinder.get_path(start, end);
    path_timer.print_time();
}

GameScene::~GameScene() {}


void GameScene::update(__attribute__((unused)) double const delta)
{
    // notice("updating game");
}


void GameScene::render(SDL_Renderer* renderer) const
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    const SDL_Rect test_rect{640 - 50, 360 - 50, 100, 100};

    SDL_RenderFillRect(renderer, &test_rect);
}

void GameScene::update_hud()
{
    // notice("game hud");
    bool panel_visible = true;
    ImGui::Begin("Game", &panel_visible);
    ImGui::Text("Be welcome to our game world!");

    if (ImGui::Button("Main Menu"))
    {
        this->scene_status = {true, MENU};
    }
    else if (ImGui::Button("Quit"))
    {
        this->scene_status = {true, QUIT};
    }

    ImGui::End();
}