#pragma once

//local
#include "./scene.hpp"

class MenuScene : public Scene
{
public:
    MenuScene();
    ~MenuScene();

    void update(const double delta);
    void render(SDL_Renderer* renderer);
    void update_hud();

};
