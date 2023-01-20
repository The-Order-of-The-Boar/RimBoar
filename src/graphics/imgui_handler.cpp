// header
#include "./imgui_handler.hpp"

// builtin
#include <iostream>

// third-party
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_sdlrenderer.h>

ImGuiHandler::ImGuiHandler(SDL_Window* window, SDL_Renderer* renderer)
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGuiIO& io{ImGui::GetIO()};

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);
}

ImGuiHandler::~ImGuiHandler()
{
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiHandler::render()
{
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    bool panel_visible = true;
    ImGui::Begin("RimBoar", &panel_visible);
    ImGui::Text("Tynan Sylvester be aware. A new Crusade is coming!");
    ImGui::End();

    ImGui::Render();
}
