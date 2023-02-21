// header
#include "renderer.hpp"

// local
#include "../logging/log.hpp"



void Renderer::move(glm::i32vec2 offset)
{
    this->offset += offset;
}



SDL_Rect make_vertical_line(glm::u32vec2 position, size_t lenght, size_t width) {

    auto line_rect = SDL_Rect{
        .x = (int32_t)position.x - (int32_t)(width / 2),
        .y = (int32_t)position.y,
        .w = (int32_t)width,
        .h = (int32_t)lenght
    };

    return line_rect;
}

SDL_Rect make_horizontal_line(glm::u32vec2 position, size_t lenght, size_t width) {

    auto line_rect = SDL_Rect{
        
        .x = (int32_t)position.x,
        .y = (int32_t)position.y - (int32_t)(width / 2),
        .w = (int32_t)lenght,
        .h = (int32_t)width
    };

    return line_rect;
}

void Renderer::render(SDL_Renderer* sdl_renderer, World const& world) const
{
    this->sdl_renderer = sdl_renderer;


    // floors and walls

    auto size = world.map.size();

    for (size_t x = 0; x < size.x; ++x)
    {
        for (size_t y = 0; y < size.y; ++y)
        {
            this->render_floor(x, y);

            if (world.map.get(x, y).wall.has_value())
                this->render_wall(x, y);
        }
    }


    // lines

    for (size_t line_x = 0; line_x < size.x; ++line_x) {

        auto pixel_x = line_x * TILE_SIZE;
        auto column_size = TILE_SIZE * size.y;

        auto line_rect = make_vertical_line(glm::i32vec2{pixel_x, 0} + this->offset, column_size, LINE_WIDTH);
        this->render_rect(line_rect, line_color);
    }

    for (size_t line_y = 0; line_y < size.y; ++line_y) {

        auto pixel_y = line_y * TILE_SIZE;
        auto line_size = (int32_t)TILE_SIZE * (int32_t)size.x;

        auto line_rect = make_horizontal_line(glm::i32vec2{0, pixel_y} + this->offset, line_size, LINE_WIDTH);
        this->render_rect(line_rect, line_color);
    }


    // units

    for (size_t tile_x = 0; tile_x < size.x; ++tile_x) {
        for (size_t tile_y = 0; tile_y < size.y; ++tile_y) {

            if (world.map.get(tile_x, tile_y).unit.has_value() == false)
                continue;

            this->render_unit(tile_x, tile_y);
        }
    }

    rb_assert(this->sdl_renderer != nullptr);
    this->sdl_renderer = nullptr;
}

void Renderer::render_rect(SDL_Rect rect, glm::u8vec3 color) const {

    rb_assert(this->sdl_renderer != nullptr);

    SDL_SetRenderDrawColor(this->sdl_renderer, color.r, color.g, color.b, 255);
    SDL_RenderDrawRect(this->sdl_renderer, &rect);
    SDL_RenderFillRect(this->sdl_renderer, &rect);
}

void Renderer::render_floor(size_t x, size_t y) const {

    rb_assert(this->sdl_renderer != nullptr);

    SDL_Rect floor_rect{
        .x = (int32_t)(x * TILE_SIZE) + this->offset.x,
        .y = (int32_t)(y * TILE_SIZE) + this->offset.y,
        .w = TILE_SIZE,
        .h = TILE_SIZE
    };

    this->render_rect(floor_rect, floor_color);
}

void Renderer::render_wall(size_t x, size_t y) const {

    SDL_Rect wall_rect{
        .x = (int32_t)(x * TILE_SIZE) + this->offset.x,
        .y = (int32_t)(y * TILE_SIZE) + this->offset.y,
        .w = TILE_SIZE,
        .h = TILE_SIZE
    };

    this->render_rect(wall_rect, wall_color);
}

void Renderer::render_unit(size_t x, size_t y) const {

    rb_assert(this->sdl_renderer != nullptr);

    auto tile_pixel_pos = glm::u32vec2{x, y} * TILE_SIZE;
    auto tile_pixel_center = tile_pixel_pos + (TILE_SIZE / 2);

    auto entity_rect = SDL_Rect{
        .x = (int32_t)tile_pixel_center.x - (int32_t)(ENTITY_SIZE / 2) + this->offset.x,
        .y = (int32_t)tile_pixel_center.y - (int32_t)(ENTITY_SIZE / 2) + this->offset.y,
        .w = ENTITY_SIZE,
        .h = ENTITY_SIZE
    };

    this->render_rect(entity_rect, entity_color);
}

