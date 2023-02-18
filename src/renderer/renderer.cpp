// header
#include "renderer.hpp"

// local
#include "../logging/log.hpp"



void Renderer::move(glm::i32vec2 offset)
{
    this->offset += offset;
}

void draw_rect(SDL_Renderer* renderer, SDL_Rect rect, glm::u8vec3 color) {

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_RenderFillRect(renderer, &rect);
}

void draw_line(SDL_Renderer* renderer, const glm::i32vec2 origin, const glm::i32vec2 target, const glm::u8vec3 color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderDrawLine(renderer, origin.x, origin.y, target.x, target.y);
}

void Renderer::render(SDL_Renderer* sdl_renderer, World const& world) const
{
    // tiles

    auto size = world.map.size();

    for (size_t x = 0; x < size.x; ++x)
    {
        for (size_t y = 0; y < size.y; ++y)
        {
            SDL_Rect tile_rect{
                .x = (int32_t)(x * TILE_SIZE) + this->offset.x,
                .y = (int32_t)(y * TILE_SIZE) + this->offset.y,
                .w = TILE_SIZE,
                .h = TILE_SIZE
            };

            glm::u8vec3 color;

            switch (world.map.get(x,y).state) 
            {
                case Tile::State::Wall:
                    color = wall_color;
                    break;
                case Tile::State::Path:
                    color = path_color;
                    break;
                default:
                    color = floor_color;
                    break;
            }
            draw_rect(sdl_renderer, tile_rect, color);
        }
    }


    // lines

    for (size_t line_x = 0; line_x < size.x; ++line_x) {

        auto pixel_x = line_x * TILE_SIZE;
        auto column_size = (int32_t)TILE_SIZE * (int32_t)size.y;

        auto line_rect = SDL_Rect{
            .x = (int32_t)pixel_x + this->offset.x - (int32_t)(LINE_WIDTH / 2),
            .y = 0 + this->offset.y,
            .w = LINE_WIDTH,
            .h = column_size
        };

        draw_rect(sdl_renderer, line_rect, line_color);
    }

    for (size_t line_y = 0; line_y < size.y; ++line_y) {

        auto pixel_y = line_y * TILE_SIZE;
        auto line_size = (int32_t)TILE_SIZE * (int32_t)size.x;

        auto line_rect = SDL_Rect{
            
            .x = 0 + this->offset.x,
            .y = (int32_t)pixel_y + this->offset.y - (int32_t)(LINE_WIDTH / 2),
            .w = line_size,
            .h = LINE_WIDTH
        };

        draw_rect(sdl_renderer, line_rect, line_color);
    }

    // entities

    for (size_t tile_x = 0; tile_x < size.x; ++tile_x) {
        for (size_t tile_y = 0; tile_y < size.y; ++tile_y) {

            if (world.map.get(tile_x, tile_y).state != Tile::State::Occupied)
                continue;

            auto tile_pixel_pos = glm::u32vec2{tile_x, tile_y} * TILE_SIZE;
            auto tile_pixel_center = tile_pixel_pos + (TILE_SIZE / 2);

            auto entity_rect = SDL_Rect{
                .x = (int32_t)tile_pixel_center.x - (int32_t)(ENTITY_SIZE / 2) + this->offset.x,
                .y = (int32_t)tile_pixel_center.y - (int32_t)(ENTITY_SIZE / 2) + this->offset.y,
                .w = ENTITY_SIZE,
                .h = ENTITY_SIZE
            };

            draw_rect(sdl_renderer, entity_rect, entity_color);
        }
    }
}
