#pragma once

// builtin
#include <glm/ext/vector_int2_sized.hpp>
#include <vector>
#include <functional>
#include <optional>
#include <memory>

// extern
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// local
#include "../data_structures/graph.hpp"
#include "../pathfinding/pathfinder.hpp"


struct Tile {

    enum class State {

        Emtpy,
        Occupied,
        Wall,
        Path,
    };

    State state;

    std::optional<float> walk_speed() const;
};

class Map {

    private:
        const int32_t LINEAR_MOVEMENT_COST = 10;
        const int32_t DIAGONAL_MOVEMENT_COST = 10;

        std::vector<std::vector<Tile>> data;
        size_t size_x;
        size_t size_y;
        
        std::unique_ptr<Graph> graph_representation;

    private:
        void update_tile_connections(const glm::i32vec2 index);
        void update_graph_representation();

    public:
        std::unique_ptr<Pathfinder> pathfinder;

    public:

        Map(glm::u32vec2 size, std::function<Tile(glm::u32vec2)> generator);

        glm::u32vec2 size() const;
        bool is_inside_boundaries(const glm::i32vec2 index) const;

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
};
