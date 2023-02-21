#pragma once

// builtin
#include <vector>

// local
#include "../core/world.hpp"
#include "../renderer/renderer.hpp"
#include "./scene.hpp"

// extern
#include <glm/vec3.hpp>



class GameScene : public Scene
{
private:

    enum InteractionMode
    {
        COMMAND,
        CONSTRUCTION,
    };

    int32_t current_interaction_mode = COMMAND;
    bool left_mouse_pressed = false;
    bool right_mouse_pressed = false;
    glm::i32vec2 hovered_index;

    std::unique_ptr<World> world = nullptr;
    Renderer renderer;


public:

    GameScene();
    ~GameScene();

    void update(double const delta) override;
    void render(SDL_Renderer* renderer) const override;
    void update_hud() override;
};