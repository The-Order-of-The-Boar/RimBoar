#pragma once

//local
#include "../graphics/graphic_manager.hpp"
#include "../scenes/scene.hpp"
#include <memory>

class GameApplication
{
private:
    bool shall_quit = false;
public:
    std::shared_ptr<Scene> current_scene;

public:
    GraphicManager graphic_manager{boarglib::Vector2i32{1280,720}};

    void run();

private:
    void handle_input();
    void change_scene(std::shared_ptr<Scene> new_scene);

};