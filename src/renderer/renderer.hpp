#pragma once

// builtin
#include <cstdint>

// extern
#include <SDL.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// local
#include "../core/world.hpp"



const uint32_t TILE_SIZE = 50;
const uint32_t ENTITY_SIZE = (TILE_SIZE / 5) * 3;
const uint32_t LINE_WIDTH = 2;
glm::u8vec3 const floor_color = {0, 200, 0};
glm::u8vec3 const wall_color = {100, 100, 100};
glm::u8vec3 const entity_color = {255, 0, 0};
glm::u8vec3 const line_color = {255, 255, 255};


class Renderer
{
private:

    glm::i32vec2 offset = {0, 0};

public:

    void move(glm::i32vec2 offset);
    void render(SDL_Renderer* sdl_renderer, World const& world) const;
};