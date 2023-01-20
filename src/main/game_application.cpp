//header
#include "./game_application.hpp"

//builtin
#include <iostream>

//third party
#include <SDL.h>
#include <memory>

//local
#include "../logging/log.hpp"
#include "../scenes/menu_scene.hpp"

void GameApplication::handle_input()
{
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            shall_quit = true;
        else if(event.type == SDL_KEYDOWN)
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

void GameApplication::change_scene(std::shared_ptr<Scene> new_scene)
{
    this->current_scene = new_scene;
    this->graphic_manager.current_hud_func = current_scene->hud_func;
    this->current_scene->setup_func();
}

void GameApplication::run()
{
    notice("RimBoar lives!");

    this->change_scene(std::make_shared<MenuScene>());


    while(!shall_quit)
    {
        this->handle_input();
        this->current_scene->update_func(0.1);
        this->graphic_manager.render();
    }
    
}