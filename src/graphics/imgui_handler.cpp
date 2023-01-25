// header
#include "./imgui_handler.hpp"
#include "../logging/log.hpp"

// builtin
#include <iostream>

// third-party
#include <SDL.h>
#include <fmt/format.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>



ImGuiHandler::ImGuiHandler(SDL_Window* window, SDL_Renderer* renderer, float scale)
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io{ImGui::GetIO()};

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.Fonts->AddFontFromFileTTF("cousine_regular.ttf", 13 * scale);
    ImGui::GetStyle().ScaleAllSizes(scale);

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);
}

ImGuiHandler::~ImGuiHandler()
{
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiHandler::update(std::function<void(void)> hud_func)
{
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    hud_func();

    ImGui::Render();
}
