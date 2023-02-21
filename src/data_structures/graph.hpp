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
    int32_t node_count;

public:
    Graph(const int32_t node_count)
        : node_count{node_count}
    {
        this->connections.resize(this->node_count);
    }

    Graph(const glm::i32vec2 world_size)
        : node_count{world_size.x * world_size.y}
    {
        this->connections.resize(this->node_count);
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

    int32_t get_node_count() const
    {
        return this->node_count;
    }

    ~Graph() = default;
};