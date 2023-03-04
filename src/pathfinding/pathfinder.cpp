// header
#include "./pathfinder.hpp"

// builtin
#include <cstddef>
#include <cstdint>
#include <glm/ext/vector_int2_sized.hpp>
#include <iostream>
#include <ostream>
#include <queue>
#include <vector>

// local
#include "../utils/logging/assert.hpp"
#include "../utils/print_utils.hpp"
#include "../utils/time_utils.hpp"
#include "graph.hpp"

void PathfindingNode::setup(const uint32_t movement_cost, const uint32_t total_cost,
                            const uint32_t origin_id)
{
    this->movement_cost = movement_cost;
    this->total_cost = total_cost;
    this->origin_id = origin_id;

    this->initialized = true;
    this->visited = false;
}

PathfindingNode* Pathfinder::get_node(const uint32_t id)
{
    return &this->node_poll.at(id);
}

void Pathfinder::reset_node_state()
{
    TimeMeasurer reset_timer{"PF state"};
    while (!this->open_list.empty())
        this->open_list.pop();

    for (auto& node: this->node_poll)
    {
        node.visited = false;
        node.initialized = false;
    }
    // reset_timer.print_time();
}

int32_t Pathfinder::manhattan_distance(const glm::u32vec2 _pos, const glm::u32vec2 _target)
{
#define LINEAR_MOVEMENT_COST 10;

    glm::i64vec2 pos = _pos;
    glm::i64vec2 target = _target;

    return (std::abs(pos.x - target.x) + std::abs(pos.y - target.y)) * LINEAR_MOVEMENT_COST;
}

Pathfinder::Pathfinder(Graph const* const graph, const glm::u32vec2 world_size):
    graph{graph}, world_size{world_size}
{
    this->node_poll.resize(world_size.x * world_size.y);
    for (size_t i = 0; i < this->node_poll.size(); i++)
    {
        this->node_poll.at(i).id = i;
        this->node_poll.at(i).index = glm::u32vec2(i / world_size.y, i % world_size.y);
    }
}

std::vector<glm::i32vec2> Pathfinder::get_path(const glm::u32vec2 origin, const glm::u32vec2 target)
{
    std::vector<glm::i32vec2> path{};

    const uint32_t origin_id = this->graph->get_id_from_index(origin);
    PathfindingNode* origin_node = &node_poll.at(origin_id);

    this->open_list.push(origin_node);
    while (this->open_list.size() > 0)
    {
        auto current_node = this->open_list.top();
        this->open_list.pop();
        current_node->visited = true;
        if (current_node->index == target)
        {
            while (current_node->index != origin)
            {
                path.push_back(current_node->index);
                current_node = this->get_node(current_node->origin_id);
            }
            goto cleanup;
        }

        auto const& connections = this->graph->get_node_connections(current_node->id);
        for (size_t i = 0; i < connections.size(); i++)
        {
            Edge const* path = &connections.at(i);
            // Test using value first
            auto node = &this->node_poll.at(path->target_node_id);
            const uint32_t new_movement_cost = current_node->movement_cost + path->cost;
            const uint32_t new_total_cost =
                new_movement_cost + Pathfinder::manhattan_distance(node->index, target);

            if (!node->initialized)
            {
                node->setup(new_movement_cost, new_total_cost, path->origin_node_id);
                this->open_list.push(node);
            }
            // On open list
            else if (!node->visited && (new_total_cost < node->total_cost))
            {
                node->setup(new_movement_cost, new_total_cost, path->origin_node_id);
            }
            // On closed list
            else if (new_total_cost < node->total_cost)
            {
                node->setup(new_movement_cost, new_total_cost, path->origin_node_id);
                this->open_list.push(node);
            }
        }
    }

cleanup:
    this->reset_node_state();
    return path;
};
