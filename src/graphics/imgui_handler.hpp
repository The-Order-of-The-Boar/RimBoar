#pragma once

class SDL_Window;
class SDL_Renderer;

class ImGuiHandler
{
public:

    ImGuiHandler(SDL_Window* window, SDL_Renderer* renderer);
    ~ImGuiHandler();

    void render();
};