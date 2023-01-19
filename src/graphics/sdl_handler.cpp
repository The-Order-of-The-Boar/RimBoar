//header
#include "./sdl_handler.hpp"

//builtin
#include <assert.h>
#include <iostream>

//third party
#include <SDL.h>
#include <imgui_impl_sdlrenderer.h>

SDLHandler::SDLHandler(const boarglib::Vector2i32 window_size)
    :window_size{window_size}
{
    const auto sdl_init_status =  SDL_Init(SDL_INIT_VIDEO);
    if(sdl_init_status != 0 )
    {
        std::cout << SDL_GetError() << std::endl;
        assert(sdl_init_status == 0);
    }

    constexpr uint32_t WINDOW_FLAGS = SDL_WINDOW_SHOWN;
    this->window = SDL_CreateWindow("RimBoar", 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            window_size.x, window_size.y, WINDOW_FLAGS);
    if (this->window == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        assert(this->window != NULL);
    }

    constexpr uint32_t RENDERER_FLAGS = SDL_RENDERER_ACCELERATED;
    this->renderer = SDL_CreateRenderer(window, -1 ,RENDERER_FLAGS);
    if( this->renderer == NULL )
    {
        std::cout << SDL_GetError() << std::endl;
        assert(this->renderer == NULL);
    }
    else{
        SDL_SetRenderDrawColor(this->renderer, 0,0,0,0);
    }

    std::cout << "Started SDLHandler\n";
}

SDLHandler::~SDLHandler()
{
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

void SDLHandler::render()
{
    SDL_SetRenderDrawColor(this->renderer, 0,0,255,255);
    SDL_RenderClear(this->renderer);

    SDL_SetRenderDrawColor(this->renderer, 255, 0, 0, 255);
    const SDL_Rect test_rect{
            this->window_size.x/2 - 50, 
            this->window_size.y/2 - 50, 100, 100};

    SDL_RenderFillRect(this->renderer, &test_rect);

    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());

    SDL_RenderPresent(this->renderer);
}