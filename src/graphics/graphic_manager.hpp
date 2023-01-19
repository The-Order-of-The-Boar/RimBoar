#pragma once

#include <data_structures/vector.hpp>

struct SDL_Window;
struct SDL_Renderer;

class SDLHandler
{
private:
    
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    boarglib::Vector2i32 window_size;

public:
    SDLHandler(const boarglib::Vector2i32 window_size);
    ~SDLHandler();

    void render();

};

class GraphicManager
{
private:
    SDLHandler sdl_handler;

public:
    bool shall_quit = false;
    GraphicManager(boarglib::Vector2i32 window_size);
    ~GraphicManager();

    void process_input();
    void render();


};