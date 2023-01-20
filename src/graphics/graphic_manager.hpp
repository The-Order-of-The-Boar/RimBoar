#pragma once

//builtin
#include <functional>

//local
#include "../data_structures/vector.hpp"
#include "./imgui_handler.hpp"
#include "./sdl_handler.hpp"


class GraphicManager
{
private:
    SDLHandler sdl_handler;
    ImGuiHandler imgui_handler;

public:
    std::function<void(void)> current_hud_func    = []{};

public:
    GraphicManager(boarglib::Vector2i32 window_size);
    ~GraphicManager();

    void render();

};