//header
#include "./pathfinder.hpp"

//builtin
#include <cstddef>
#include <cstdint>
#include <glm/ext/vector_int2_sized.hpp>
#include <iostream>
#include <ostream>
#include <queue>
#include <vector>

//local
#include "../logging/assert.hpp"
#include "../utils/print_utils.hpp"
#include "../data_structures/graph.hpp"

void PathfindingNode::setup(const int32_t movement_cost, const int32_t total_cost, const int32_t origin_id)
{
    this->movement_cost = movement_cost;
    this->total_cost = total_cost;
    this->origin_id = origin_id;

    this->initialized = true;
    this->visited = false;
}

PathfindingNode* Pathfinder::get_node(const int32_t id)
{
    return &this->node_poll.at(id);
}


int32_t Pathfinder::manhattan_distance(const glm::i32vec2 pos, const glm::i32vec2 target)
{
    return (abs(pos.x - target.x) + abs(pos.y - target.y)) * 10;
}

Pathfinder::Pathfinder(const Graph* const graph, const glm::i32vec2 world_size)
    :graph{graph}, world_size{world_size}
{
    this->node_poll.resize(world_size.x * world_size.y);
    for(int32_t i = 0; i < this->node_poll.size(); i++)
    {
        this->node_poll.at(i).id = i;
        this->node_poll.at(i).index = glm::i32vec2(i/world_size.x, i%world_size.x);
    }
}

std::vector<glm::i32vec2> Pathfinder::get_path(const glm::i32vec2 origin, const glm::i32vec2 target)
{
    std::vector<glm::i32vec2> path{};

    const int32_t origin_id = origin.x * this->world_size.x + origin.y;
    PathfindingNode* origin_node = &node_poll.at(origin_id);

    this->open_list.push(origin_node);

    while(this->open_list.size() > 0)
    {
        auto current_node = this->open_list.top();
        this->open_list.pop();
        current_node->visited = true;

        if(current_node->index == target)
        {
            while(current_node->index != origin)
            {
                path.push_back(current_node->index);
                current_node = this->get_node(current_node->origin_id);
                std::cout << current_node;
            }
            return path;
        }

        const auto& connections = this->graph->get_node_connections(current_node->id);
        for(size_t i = 0; i < connections.size(); i++)
        {
            const Edge* path = &connections.at(i);
            // Test using value first
            auto node = &this->node_poll.at(path->target_node_id);
            const int32_t new_movement_cost = current_node->movement_cost + path->cost;
            const int32_t new_total_cost = new_movement_cost + Pathfinder::manhattan_distance(node->index, target);
            
            if(!node->initialized)
            {
                node->setup(new_movement_cost, new_total_cost, path->origin_node_id);
                this->open_list.push(node);
            }
            //On open list
            else if(!node->visited && (new_total_cost < node->total_cost))
            {
                node->setup(new_movement_cost, new_total_cost, path->origin_node_id);
            }
            //On closed list
            else if(new_total_cost < node->total_cost)
            {
                node->setup(new_movement_cost, new_total_cost, path->origin_node_id);
                this->open_list.push(node);
            }
        }
    }


    return path;
};


