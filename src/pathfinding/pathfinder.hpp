#pragma once

// builtin
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>
#include <optional>

// third party
#include <glm/ext/scalar_int_sized.hpp>
#include <glm/ext/vector_int2_sized.hpp>
#include <glm/vec2.hpp>

class Graph;

struct PathfindingNode
{
    glm::u32vec2 index;
    uint32_t origin_id = -1;
    uint32_t id = 0;

    uint32_t movement_cost = 0;
    uint32_t total_cost = 0;
    bool visited = false;
    bool initialized = false;


    void setup(const uint32_t movement_cost, const uint32_t total_cost, const uint32_t origin_id);
};

template <>
struct std::less<PathfindingNode*>
{
    bool operator()(PathfindingNode* const& a, PathfindingNode* const& b)
    {
        return a->total_cost > b->total_cost;
    }
};

class Pathfinder
{
private:

    Graph const* const graph;
    const glm::u32vec2 world_size;

    std::vector<PathfindingNode> node_poll;
    std::priority_queue<PathfindingNode*, std::vector<PathfindingNode*>> open_list{};

private:

    PathfindingNode* get_node(const uint32_t id);
    void reset_node_state();


public:

    Pathfinder(Graph const* const graph, const glm::u32vec2 world_size);
    ~Pathfinder() = default;

    static int32_t manhattan_distance(const glm::u32vec2 pos, const glm::u32vec2 target);
    std::optional<std::vector<glm::i32vec2>> get_path(const glm::u32vec2 origin, glm::u32vec2 target);
};
