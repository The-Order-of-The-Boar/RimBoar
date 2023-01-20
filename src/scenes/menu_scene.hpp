#pragma once

//local
#include "./scene.hpp"

class MenuScene : public Scene
{
public:
    MenuScene();

    void setup();

    void update(const double delta);
    void update_hud();

};
