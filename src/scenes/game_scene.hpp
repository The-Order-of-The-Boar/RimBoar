#pragma once

// builtin
#include <vector>

// local
#include "./scene.hpp"
#include "../core/world.hpp"
#include "../renderer/renderer.hpp"

// extern
#include <glm/vec3.hpp>



class GameScene : public Scene
{
private:

    std::unique_ptr<World> world = nullptr;
    Renderer renderer;

public:

    GameScene();
    ~GameScene();

    void update(double const delta) override;
    void render(SDL_Renderer* renderer) const override;
    void update_hud() override;
};