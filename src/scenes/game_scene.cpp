#include "./game_scene.hpp"

// builtin
#include <vector>

// extern
#include <SDL.h>
#include <SDL_stdinc.h>
#include <imgui.h>
#include <glm/vec2.hpp>

// local
#include "../logging/log.hpp"
#include "../logging/assert.hpp"
#include "scene.hpp"













Map create_map(glm::u32vec2 size) {

    auto output = Map{size, [&](glm::u32vec2){ return Tile{.state = Tile::State::Emtpy }; }};
    return output;
}

GameScene::GameScene() {

    this->world = std::unique_ptr<World>{new World{.map = create_map({10, 10})}};
    
    this->world->map.get(1, 1).state = Tile::State::Occupied;

    for (size_t y = 1; y < 9; ++y)
        this->world->map.get(3, y).state = Tile::State::Wall;
}

GameScene::~GameScene() {}



void GameScene::update(double const delta)
{
    (void)delta;

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