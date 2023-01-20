#pragma once

//builtin
#include <optional>
#include <functional>
#include <memory>

class Scene
{
    struct SceneFinalizationStatus
    {
        bool close_scene = false;
        std::optional< std::shared_ptr<Scene>> next_scene;
    };


public:
    SceneFinalizationStatus scene_status{};

    std::function<void(void)> setup_func; //Executed once when scene is loaded

    //Executed each frame, no access to rendering
    std::function<void(const double)> update_func; 

    //TODO
    //Structure to hold game objects to be rendered on SDLHandler    

    //Executed each frame inside ImGuiHandler, acess to ImGui rendering
    std::function<void(void)> hud_func;
};


