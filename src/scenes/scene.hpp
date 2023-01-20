#pragma once

//builtin
#include <optional>
#include <functional>
#include <memory>
#include <iostream>

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
    std::function<void(const double)> update_func; 

    //TODO
    //Structure to hold game objects to be rendered on SDLHandler    

    //Executed each frame inside ImGuiHandler, acess to ImGui rendering
    std::function<void(void)> hud_func;
};

