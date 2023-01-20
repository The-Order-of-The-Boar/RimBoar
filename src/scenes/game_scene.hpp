#pragma once

//local 
#include "./scene.hpp"

class GameScene : public Scene
{
public:
    GameScene();
    ~GameScene();

    void update(const double delta);
    void render(SDL_Renderer* renderer);
    void update_hud();
};