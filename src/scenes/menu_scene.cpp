//header
#include "./menu_scene.hpp"

//third party
#include <imgui.h>

//local
#include "../logging/log.hpp"
#include "scene.hpp"

MenuScene::MenuScene()
{
    this->update_func = std::bind(&MenuScene::update, this, std::placeholders::_1);
    this->hud_func = std::bind(&MenuScene::update_hud, this);
    notice("Menu scene started");
}

MenuScene::~MenuScene()
{
    notice("Menu scene freed");
}


void MenuScene::update(const double delta)
{
    //notice("updating menu");
}

void MenuScene::update_hud()
{
    //notice("menu hud");
    bool panel_visible = true;
    ImGui::Begin("RimBoar",&panel_visible);
    ImGui::Text("Tynan Sylvester be aware. A new Crusade is coming!");
    
    if(ImGui::Button("Play"))
    {
        this->scene_status.close_scene = true;
        this->scene_status.next_scene = GAME;
    }
    else if(ImGui::Button("Quit"))
    {
        this->scene_status.close_scene = true;
        this->scene_status.next_scene = QUIT;
    }


    ImGui::End();
}