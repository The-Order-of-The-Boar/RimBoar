#include "./game_scene.hpp"

//third party
#include <imgui.h>

//local
#include "../logging/log.hpp"
#include "scene.hpp"

GameScene::GameScene()
{
    this->update_func = std::bind(&GameScene::update, this, std::placeholders::_1);
    this->hud_func = std::bind(&GameScene::update_hud, this);
    notice("Game scene started");
}

GameScene::~GameScene()
{
    notice("Game Scene freed");
}


void GameScene::update(const double delta)
{
    //notice("updating game");
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