#pragma once

// builtin
#include <array>
#include <cstdint>
#include <glm/ext/vector_int2_sized.hpp>
#include <vector>

// third party
#include <glm/vec2.hpp>

struct Edge
{
    uint32_t origin_node_id;
    uint32_t target_node_id;
    uint32_t cost;
};

class Graph
{
private:

    std::vector<std::vector<Edge>> connections;
    uint32_t node_count;
    const uint32_t column_size;

public:

    Graph(const glm::u32vec2 world_size): node_count{world_size.x * world_size.y}, column_size(world_size.y)
    {
        this->connections.resize(this->node_count);
    }

    void add_node_connection(const uint32_t origin_node_id, const uint32_t target_node_id,
                             const uint32_t cost)
    {
        auto new_edge = Edge{origin_node_id, target_node_id, cost};
        this->connections.at(origin_node_id).push_back(std::move(new_edge));
    }

    void reset_node_connections(const uint32_t node_id)
    {
        this->connections.at(node_id).resize(0);
    }

    std::vector<Edge> get_node_connections(const uint32_t node_id) const
    {
        return this->connections.at(node_id);
    }

    uint32_t get_node_count() const
    {
        return this->node_count;
    }

    uint32_t get_id_from_index(glm::u32vec2 const index) const {

        return index.x * this->column_size + index.y;
    }

    ~Graph() = default;
};