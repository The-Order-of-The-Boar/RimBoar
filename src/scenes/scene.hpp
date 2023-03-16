#pragma once

// builtin
#include <functional>
#include <iostream>
#include <memory>
#include <optional>

struct SDL_Renderer;
class AudioManager;

enum SceneID
{
    QUIT,
    MENU,
    GAME,
};

struct SceneFinalizationStatus
{
    bool close_scene = false;
    SceneID next_scene = QUIT;
};

class Scene
{
public:

    SceneFinalizationStatus scene_status{};
    AudioManager* const audio_manager;

public:

    Scene(AudioManager* const audio_manager): audio_manager{audio_manager} {}

    virtual ~Scene(){};

    // Executed each frame, no access to rendering
    virtual void update(double const delta) = 0;

    // Executed each frame inside SDL_Handler, access to SDL rendering
    virtual void render(SDL_Renderer* renderer) const = 0;

    // Executed each frame inside ImGuiHandler, access to ImGui rendering
    virtual void update_hud() = 0;
};
