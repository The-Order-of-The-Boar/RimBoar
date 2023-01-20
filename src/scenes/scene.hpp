#pragma once

//builtin
#include <optional>
#include <functional>
#include <memory>
#include <iostream>

struct SDL_Renderer;

enum SceneID
{
    QUIT,
    MENU,
    GAME,
};

struct SceneFinalizationStatus
{
    bool close_scene = false;
    SceneID next_scene = QUIT;
};

class Scene
{
public:
    SceneFinalizationStatus scene_status{};

    //Executed each frame, no access to rendering
    virtual void update(const double delta){};

    //Executed each frame inside SDL_Handler, access to SDL rendering
    virtual void render(SDL_Renderer* renderer){};

    //Executed each frame inside ImGuiHandler, access to ImGui rendering
    virtual void update_hud(){};
};

