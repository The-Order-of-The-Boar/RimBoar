#pragma once

// extern
#include <glm/vec2.hpp>


struct SDL_Window;
struct SDL_Renderer;

class SDLHandler
{
public:
    glm::i32vec2 window_size;
    SDL_Window* window;
    SDL_Renderer* renderer;

public:
    SDLHandler(const glm::i32vec2 window_size);
    ~SDLHandler();

    void render();

};

