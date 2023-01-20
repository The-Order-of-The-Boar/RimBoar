//header
#include "graphic_manager.hpp"

//builtin
#include <iostream>

GraphicManager::GraphicManager(boarglib::Vector2i32 window_size)
    :sdl_handler{window_size}, 
     imgui_handler(sdl_handler.window, sdl_handler.renderer)
{

}

GraphicManager::~GraphicManager()
{

}

void GraphicManager::render()
{
    //SDL must render after ImGui, because ImGui actually only creates the frame that is drawn bt SDL
    this->imgui_handler.update(this->hud_func);
    this->sdl_handler.render(this->render_func);
}
