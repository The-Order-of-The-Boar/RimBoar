// header
#include "./menu_scene.hpp"

// third party
#include <SDL.h>
#include <imgui.h>

// local
#include "../utils/logging/log.hpp"
#include "scene.hpp"

#include <chrono>
#include <thread>

MenuScene::MenuScene() {}

MenuScene::~MenuScene() {}


void MenuScene::update(double const delta)
{
    (void)delta;
    // notice("updating menu");
}

void MenuScene::render(SDL_Renderer* renderer) const
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

void MenuScene::update_hud()
{
    // notice("menu hud");
    bool panel_visible = true;
    ImGui::Begin("RimBoar", &panel_visible);
    ImGui::Text("Tynan Sylvester be aware. A new Crusade is coming!");

    if (ImGui::Button("Play"))
    {
        this->scene_status.close_scene = true;
        this->scene_status.next_scene = GAME;
    }
    else if (ImGui::Button("Quit"))
    {
        this->scene_status.close_scene = true;
        this->scene_status.next_scene = QUIT;
    }

    ImGui::End();
}