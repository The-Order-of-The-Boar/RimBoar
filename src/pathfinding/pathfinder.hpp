#pragma once

//builtin
#include <cstddef>
#include <cstdint>
#include <queue>
#include <iostream>

//third party
#include <glm/vec2.hpp>
#include <glm/ext/scalar_int_sized.hpp>
#include <glm/ext/vector_int2_sized.hpp>
#include <vector>

//local
#include "../data_structures/graph.hpp"

//TEMPORARY CODE TO TEST PATHFINDING
    struct TempTile
    {
        bool empty = true;
        glm::i32vec2 index;
        int32_t id;
    };

    class TempWorld
    {
    private:
        const int linear_move_cost = 10;
        const int diagonal_move_cost = 14;

    public:
        const int32_t world_size;
        std::vector<std::vector<TempTile>> world_tiles;
        Graph connection_graph{static_cast<int32_t>(world_size*world_size)};

    public:
        TempWorld(const int32_t world_size);
        void update_tile_connections(const TempTile* tile);
        void update_connections();
        void set_tile_empty(const glm::i32vec2 index, const bool empty);


        bool is_inside_boundaries(const glm::i32vec2 index) const;
        TempTile* get_tile(const glm::i32vec2 index);

        int32_t index_to_id(const glm::i32vec2 id) const;
        glm::i32vec2 id_to_index(const int32_t id) const;

    };
//TEMPORARY CODE TO TEST PATHFINDING

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
    const Graph* graph;
    const int32_t world_size;

    constexpr static auto pathfinding_node_compare = [](const PathfindingNode* left,const PathfindingNode* right)
    {
        return left->total_cost > right->total_cost;
    };


    std::vector<PathfindingNode> node_poll;
    std::priority_queue
        <PathfindingNode*, std::vector<PathfindingNode*>, decltype(pathfinding_node_compare)> 
        open_list{pathfinding_node_compare};

private:
    PathfindingNode* get_node(const int32_t id);
    

public:
    Pathfinder(const Graph* graph, const int32_t world_size);

    static int32_t manhattan_distance(const glm::i32vec2 pos, const glm::i32vec2 target);
    std::vector<glm::i32vec2> get_path(const glm::i32vec2 origin, glm::i32vec2 target);

};



