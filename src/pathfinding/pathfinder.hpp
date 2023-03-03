#pragma once

// builtin
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <queue>

// third party
#include <glm/ext/scalar_int_sized.hpp>
#include <glm/ext/vector_int2_sized.hpp>
#include <glm/vec2.hpp>
#include <vector>

class Graph;

struct PathfindingNode
{
    glm::i32vec2 index;
    int32_t origin_id = -1;
    int32_t id = 0;

    int32_t movement_cost = 0;
    int32_t total_cost = 0;
    bool visited = false;
    bool initialized = false;


    void setup(const int32_t movement_cost, const int32_t total_cost, const int32_t origin_id);
};


class Pathfinder
{
private:

    Graph const* const graph;
    const glm::i32vec2 world_size;

    constexpr static auto pathfinding_node_compare =
        [](PathfindingNode const* left, PathfindingNode const* right)
    { return left->total_cost > right->total_cost; };


    std::vector<PathfindingNode> node_poll;
    std::priority_queue<PathfindingNode*, std::vector<PathfindingNode*>,
                        decltype(pathfinding_node_compare)>
        open_list{pathfinding_node_compare};

private:

    PathfindingNode* get_node(const int32_t id);
    void reset_node_state();


public:

    Pathfinder(Graph const* const graph, const glm::i32vec2 world_size);
    ~Pathfinder() = default;

    static int32_t manhattan_distance(const glm::i32vec2 pos, const glm::i32vec2 target);
    std::vector<glm::i32vec2> get_path(const glm::i32vec2 origin, glm::i32vec2 target);
};
