// header
#include "./sdl_handler.hpp"

// builtin
#include <SDL_error.h>
#include <assert.h>
#include <iostream>

// third party
#include <SDL.h>
#include <fmt/format.h>
#include <imgui_impl_sdlrenderer.h>

// local
#include "../application/config.hpp"
#include "../utils/logging/assert.hpp"
#include "../utils/logging/log.hpp"
#include "imgui_handler.hpp"



SDLHandler::SDLHandler(const glm::i32vec2 window_size): window_size{window_size}
{
    std::cout << "arg\n";
    auto const sdl_init_status = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    rb_runtime_assert(sdl_init_status == 0, SDL_GetError());

    uint32_t const WINDOW_FLAGS = SDL_WINDOW_SHOWN;
    this->window = SDL_CreateWindow("RimBoar", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                    window_size.x, window_size.y, WINDOW_FLAGS);
    rb_runtime_assert(this->window != NULL, SDL_GetError());

    uint32_t const RENDERER_FLAGS = 0;
    this->renderer = SDL_CreateRenderer(window, -1, RENDERER_FLAGS);
    rb_runtime_assert(this->renderer != NULL, SDL_GetError());

    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 0);

    auto dpi = this->get_dpi(this->window);
    this->display_scale = dpi / config->default_dpi;

    notice(fmt::format("display dpi: {}", dpi));
    notice(fmt::format("application scale: {}", this->display_scale));
}

SDLHandler::~SDLHandler()
{
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

void SDLHandler::render(std::function<void(SDL_Renderer*)> render_func)
{
    render_func(this->renderer);

    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderPresent(this->renderer);
}

float SDLHandler::get_dpi(SDL_Window* window)
{
    auto dindex = SDL_GetWindowDisplayIndex(window);
    float diagonal;
    SDL_GetDisplayDPI(dindex, &diagonal, nullptr, nullptr);
    return diagonal;
}
