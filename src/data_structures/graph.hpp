#pragma once

//builtin
#include <cstdint>
#include <array>
#include <glm/ext/vector_int2_sized.hpp>
#include <vector>

//third party
#include <glm/vec2.hpp>

struct Edge
{
    int32_t origin_node_id;
    int32_t target_node_id;
    int32_t cost;
};

class Graph 
{
private:
    std::vector<std::vector<Edge>> connections;

public:
    Graph(const int32_t node_count)
    {
        this->connections.resize(node_count);
    }

    Graph(const glm::i32vec2 world_size)
    {
        this->connections.resize(world_size.x * world_size.y);
    }

    void add_node_connection(const int32_t origin_node_id, const int32_t target_node_id, const int32_t cost)
    {
        this->connections.at(origin_node_id).emplace_back(origin_node_id, target_node_id, cost);
    }

    void reset_node_connections(const int32_t node_id)
    {
        this->connections.at(node_id).resize(0);
    }

    std::vector<Edge> get_node_connections(const int32_t node_id) const
    {
        return this->connections.at(node_id);
    }
};