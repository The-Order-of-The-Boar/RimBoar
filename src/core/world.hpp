#pragma once

// builtin
#include <vector>
#include <functional>
#include <optional>

// local
#include "entity_manager.hpp"

// extern
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>




struct Tile {

    std::optional<WallID> wall = std::nullopt;
    std::optional<UnitID> unit = std::nullopt;

    std::optional<float> walk_speed() const;
    bool is_occupied() const;
};

class Map {

    private:

        std::vector<std::vector<Tile>> data;
        size_t size_x;
        size_t size_y;

    public:

        Map(glm::u32vec2 size, std::function<Tile(glm::u32vec2)> generator);

        glm::u32vec2 size() const;

        Tile& get(size_t x, size_t y);
        Tile const& get(size_t x, size_t y) const;
        Tile& get(glm::u32vec2 position);
        Tile const& get(glm::u32vec2 position) const;

        Map(Map&&) = default;
        Map& operator=(Map&&) = default;
        Map(Map const&) = delete;
        Map& operator=(Map const&) = delete;
};

struct World {

    Map map;
    EntityManager entities;

public:

    void push_unit(Unit unit, glm::u32vec2 position);
    void push_wall(Wall wall, glm::u32vec2 position);
};
