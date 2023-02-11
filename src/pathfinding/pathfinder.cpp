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

Pathfinder::Pathfinder(const Graph* graph, const int32_t world_size)
    :graph{graph}, world_size{world_size}
{
    this->node_poll.resize(world_size * world_size);
    for(int32_t i = 0; i < this->node_poll.size(); i++)
    {
        this->node_poll.at(i).id = i;
        this->node_poll.at(i).index = glm::i32vec2(i/world_size, i%world_size);
    }
}

std::vector<glm::i32vec2> Pathfinder::get_path(const glm::i32vec2 origin, const glm::i32vec2 target)
{
    std::vector<glm::i32vec2> path{};

    const int32_t origin_id = origin.x * this->world_size + origin.y;
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
                std::cout << current_node->index;
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




//TEMPORARY CODE TO TEST PATHFINDING
    TempWorld::TempWorld()
        :world_size{static_cast<int32_t>(world_tiles.size())}
    {

        rb_assert(world_tiles.size() == world_tiles.at(0).size());

        for(int32_t x = 0; x < world_size; x++)
        {
            for(int32_t y = 0; y < world_size; y++)
            {
                this->world_tiles[x][y].index = glm::i32vec2{x,y};
                this->world_tiles[x][y].id = x * world_size + y;
            }
        }
        this->update_connections();
    }

    void TempWorld::update_tile_connections(const TempTile* tile)
    {


        connection_graph.reset_node_connections(tile->id);
        for(int x = -1; x < 2; x++)
        {
            for(int y = -1; y < 2; y++)
            {
                const glm::i32vec2 dir{x,y};
                const glm::i32vec2 neighbor_pos = tile->index + dir;
                
                if(!tile->empty) continue;
                if(!this->is_inside_boundaries(neighbor_pos) || neighbor_pos == tile->index) continue;

                const TempTile* neighbor = this->get_tile(neighbor_pos);

                if(!neighbor->empty) continue;
                if(abs(dir.x) + abs(dir.y) == 2)
                {
                    if(!this->get_tile(tile->index + glm::i32vec2{dir.x,0})->empty) continue;
                    if(!this->get_tile(tile->index + glm::i32vec2{0,dir.y})->empty) continue;
                }


                const int32_t movement_cost = (dir.x == 0 || dir.y == 0) ? 10 : 14;
                connection_graph.add_node_connection(tile->id, neighbor->id, movement_cost);
            }
        }
    }

    void TempWorld::update_connections()
    {

        for(size_t x = 0; x < 100; x++)
        {
            for(size_t y = 0; y < 100; y++)
            {
                this->update_tile_connections(&this->world_tiles.at(x).at(y));
            }
        }
    }

    void TempWorld::set_tile_empty(const glm::i32vec2 index, const bool empty)
    {
        this->world_tiles[index.x][index.y].empty = empty;
    }

    bool TempWorld::is_inside_boundaries(const glm::i32vec2 index) const
    {
        return index.x >= 0 && index.x < this->world_size &&
            index.y >= 0 && index.y < this->world_size;
    }

    TempTile* TempWorld::get_tile(const glm::i32vec2 index)
    {
        return &this->world_tiles.at(index.x).at(index.y);
    }

    int32_t TempWorld::index_to_id(const glm::i32vec2 index) const
    {
        return index.x * this->world_size + index.y;
    }

    glm::i32vec2 TempWorld::id_to_index(const int32_t index) const
    {
        return glm::i32vec2{index/world_size, index%world_size};
    }

//TEMPORARY CODE TO TEST PATHFINDING
