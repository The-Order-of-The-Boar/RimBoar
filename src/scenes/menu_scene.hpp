#pragma once

// local
#include "./scene.hpp"

class MenuScene : public Scene
{
private:

    float volume;

public:

    MenuScene(AudioManager* const audio_manager);
    ~MenuScene();

    void update(double const delta) override;
    void render(SDL_Renderer* renderer) const override;
    void update_hud() override;
};
