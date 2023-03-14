// header
#include "./menu_scene.hpp"

// third party
#include <SDL.h>
#include <SDL_audio.h>
#include <SDL_mixer.h>
#include <imgui.h>

// local
#include "../logging/log.hpp"
#include "scene.hpp"
#include "../audio/audio_manager.hpp"


MenuScene::MenuScene(AudioManager* const audio_manager) : Scene{audio_manager}
{
    this->audio_manager->play_music("menu.mp3");
}

MenuScene::~MenuScene() {}


void MenuScene::update(double const delta)
{
    (void)delta;
    // notice("updating menu");
}

void MenuScene::render(SDL_Renderer* renderer) const
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

void MenuScene::update_hud()
{
    // notice("menu hud");
    bool panel_visible = true;
    ImGui::Begin("RimBoar", &panel_visible);
    ImGui::Text("Tynan Sylvester be aware. A new Crusade is coming!");

    if (ImGui::Button("Play"))
    {
        this->scene_status.close_scene = true;
        this->scene_status.next_scene = GAME;
    }
    if (ImGui::Button("Quit"))
    {
        this->scene_status.close_scene = true;
        this->scene_status.next_scene = QUIT;
    }
    if (ImGui::Button("Sound"))
    {
        this->audio_manager->play_sound("audio1.wav");
    }

    // Audio 
    if(this->audio_manager->is_muted())
    {
        if(ImGui::Button("Unmute"))
            this->audio_manager->unmute();
    }
    else
    {
        if(ImGui::Button("Mute"))
            this->audio_manager->mute();
    }

    if(ImGui::SliderFloat("Volume", &this->volume , 0, 1))
    {
        this->audio_manager->set_general_volume(this->volume);
    }



    ImGui::End();
}