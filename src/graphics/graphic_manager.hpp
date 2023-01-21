#pragma once

//builtin
#include <functional>

//local
#include "./imgui_handler.hpp"
#include "./sdl_handler.hpp"

// extern
#include <glm/vec2.hpp>



class GraphicManager
{
private:

    SDLHandler sdl_handler;
    ImGuiHandler imgui_handler;

public:
    std::function<void(SDL_Renderer*)> render_func = [](SDL_Renderer*){};
    std::function<void(void)> hud_func = []{};

public:

    GraphicManager(glm::i32vec2 window_size);
    ~GraphicManager();

    void render();
};