#pragma once

//local 
#include "./scene.hpp"

class GameScene : public Scene
{
public:
    GameScene();
    ~GameScene();

    void update(const double delta);
    void update_hud();
};