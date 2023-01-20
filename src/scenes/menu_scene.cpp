//header
#include "./menu_scene.hpp"

//third party
#include <imgui.h>

//local
#include "../logging/log.hpp"

MenuScene::MenuScene()
{
    this->setup_func = std::bind(&MenuScene::setup, this);
    this->update_func = std::bind(&MenuScene::update, this, std::placeholders::_1);
    this->hud_func = std::bind(&MenuScene::update_hud, this);
}


void MenuScene::setup()
{
    notice("Menu was setuped");
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
    ImGui::End();
}