#pragma once

// builtin
#include <functional>

class SDL_Window;
class SDL_Renderer;

class ImGuiHandler
{
public:

    ImGuiHandler(SDL_Window* window, SDL_Renderer* renderer);
    ~ImGuiHandler();

    void update(std::function<void(void)> hud_func);
};