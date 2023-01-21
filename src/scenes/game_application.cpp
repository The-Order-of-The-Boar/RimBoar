// header
#include "./game_application.hpp"

// builtin
#include <iostream>
#include <memory>
#include <functional>

// third party
#include <SDL.h>
#include <imgui_impl_sdl.h>

//local
#include "../logging/log.hpp"
#include "../scenes/menu_scene.hpp"
#include "../scenes/game_scene.hpp"
#include "../utils/time_utils.hpp"

void GameApplication::handle_input()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if(event.type == SDL_QUIT)
            shall_quit = true;
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                    shall_quit = true;
                    continue;

                default:
                    continue;
            }
        }
    }
}

void GameApplication::change_scene(const SceneID scene_id)
{
    std::unique_ptr<Scene> new_scene;
    switch(scene_id)
    {
        case QUIT:
            this->shall_quit = true;
            return;
        case MENU:
            this->current_scene = std::make_unique<MenuScene>();
            break;
        case GAME:
            this->current_scene = std::make_unique<GameScene>();
            break;
        default:
            panic("Invaid Scene id");
    }
    
    this->graphic_manager.render_func = std::bind(&Scene::render, this->current_scene.get(), std::placeholders::_1);
    this->graphic_manager.hud_func = std::bind(&Scene::update_hud, this->current_scene.get());
}

void GameApplication::run()
{
    notice("RimBoar lives!");

    this->change_scene(MENU);

    TimeMeasurer frame_timer{"",TimeMeasurer::SECOND};


    while(!shall_quit)
    {
        const double delta_time = frame_timer.get_time();
        frame_timer.restart();

        this->handle_input();

        this->current_scene->update(delta_time);
        this->graphic_manager.render();

        if(this->current_scene->scene_status.close_scene)
            this->change_scene(this->current_scene->scene_status.next_scene);
    }
}