#pragma once

// builtin
#include <functional>
#include <glm/ext/vector_int2_sized.hpp>
#include <memory>
#include <optional>
#include <vector>

// extern
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

// local
#include "../slotmap/slotmap.hpp"
#include "../pathfinding/graph.hpp"
#include "../pathfinding/pathfinder.hpp"



class Unit
{
};

struct UnitID
{
    SlotMap<Unit>::Key key;
};



class Wall
{
};

struct WallID
{
    SlotMap<Wall>::Key key;
};



struct Tile
{
    std::optional<SlotMap<Wall>::Key> wall = std::nullopt;
    std::optional<SlotMap<Unit>::Key> unit = std::nullopt;

    std::optional<float> walk_speed() const;
    bool is_occupied() const;
};

class Map
{
private:

    static const int32_t LINEAR_MOVEMENT_COST = 10;
    static const int32_t DIAGONAL_MOVEMENT_COST = 14;

    std::vector<std::vector<Tile>> data;
    size_t size_x;
    size_t size_y;

private:

    void update_tile_connections(const glm::u32vec2 index);

public:

    std::unique_ptr<Graph> graph_representation;
    std::unique_ptr<Pathfinder> pathfinder;

    // Temporary
    std::vector<glm::i32vec2> test_path;
    glm::i32vec2 test_entity_index{1, 1};
    glm::i32vec2 test_target;

public:

    Map(glm::u32vec2 size, std::function<Tile(glm::u32vec2)> generator);

    glm::u32vec2 size() const;
    bool is_inside_boundaries(const glm::u32vec2 index) const;
    void update_graph_representation();

    Tile& get(size_t x, size_t y);
    Tile const& get(size_t x, size_t y) const;
    Tile& get(glm::u32vec2 position);
    Tile const& get(glm::u32vec2 position) const;

    Map(Map&&) = default;
    Map& operator=(Map&&) = default;
    Map(Map const&) = delete;
    Map& operator=(Map const&) = delete;
};

struct World
{
    Map map;
    
    struct {
        
        SlotMap<Unit> units;
        SlotMap<Wall> walls;

    } entities;

public:

    void push_unit(Unit unit, glm::u32vec2 position);
    void push_wall(Wall wall, glm::u32vec2 position);

    void remove_unit(glm::u32vec2 position);
    void remove_wall(glm::u32vec2 position);
};
