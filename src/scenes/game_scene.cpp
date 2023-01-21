#include "./game_scene.hpp"

//third party
#include <imgui.h>
#include <SDL.h>

//local
#include "../logging/log.hpp"
#include "scene.hpp"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}


void GameScene::update(__attribute__((unused)) const double delta)
{
    //notice("updating game");
}


void GameScene::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0,0,255,255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    const SDL_Rect test_rect{
            640 - 50, 
            360 - 50,
            100, 100};

    SDL_RenderFillRect(renderer, &test_rect);
}

void GameScene::update_hud()
{
    //notice("game hud");
    bool panel_visible = true;
    ImGui::Begin("Game",&panel_visible);
    ImGui::Text("Be welcome to our game world!");
    
    if(ImGui::Button("Main Menu"))
    {
        this->scene_status = {true, MENU};
    }
    else if(ImGui::Button("Quit"))
    {
        this->scene_status = {true, QUIT};
    }

    ImGui::End();
}