#pragma once

// local
#include "./scene.hpp"

class MenuScene : public Scene
{
public:

    MenuScene();
    ~MenuScene();

    void update(double const delta);
    void render(SDL_Renderer* renderer) const;
    void update_hud();
};
