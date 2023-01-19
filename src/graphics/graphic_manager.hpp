#pragma once

#include <data_structures/vector.hpp>

struct SDL_Window;
struct SDL_Renderer;

class SDLHandler
{
public:
    boarglib::Vector2i32 window_size;
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    SDLHandler(const boarglib::Vector2i32 window_size);
    ~SDLHandler();

    void render();

};

class ImGuiHandler
{
public:
    ImGuiHandler(SDL_Window*  window, SDL_Renderer* renderer);
    ~ImGuiHandler();

    void render();
};

class GraphicManager
{
private:
    SDLHandler sdl_handler;
    ImGuiHandler imgui_handler;

public:
    GraphicManager(boarglib::Vector2i32 window_size);
    ~GraphicManager();

    void render();


};