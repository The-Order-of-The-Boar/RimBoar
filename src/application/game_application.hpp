#pragma once

// builtin
#include <memory>

// local
#include "../graphics/graphic_manager.hpp"
#include "../scenes/scene.hpp"


class GameApplication
{
private:

    bool shall_quit = false;

public:

    std::unique_ptr<Scene> current_scene = nullptr;

public:

    std::unique_ptr<GraphicManager> graphic_manager = nullptr;

    void setup();
    void run();

private:

    void handle_input();
    void change_scene(const SceneID scene_id);
};