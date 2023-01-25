#pragma once

// builtin
#include <functional>


// extern
#include <glm/vec2.hpp>


struct SDL_Window;
struct SDL_Renderer;

class SDLHandler
{
public:

    glm::i32vec2 window_size;
    float display_scale;
    SDL_Window* window;
    SDL_Renderer* renderer;

public:

    SDLHandler(const glm::i32vec2 window_size);
    ~SDLHandler();

    void render(std::function<void(SDL_Renderer*)> render_func);
    float get_dpi(SDL_Window* window);
};
