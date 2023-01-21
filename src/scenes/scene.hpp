#pragma once

// builtin
#include <functional>
#include <iostream>
#include <memory>
#include <optional>

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

    // Executed each frame, no access to rendering
    virtual void update(__attribute__((unused)) double const delta){};

    // Executed each frame inside SDL_Handler, access to SDL rendering
    virtual void render(__attribute__((unused)) SDL_Renderer* renderer){};

    // Executed each frame inside ImGuiHandler, access to ImGui rendering
    virtual void update_hud(){};
};
