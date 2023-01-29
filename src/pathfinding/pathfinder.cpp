//header
#include "./pathfinder.hpp"

//builtin
#include <cstdint>
#include <ostream>
#include <vector>

//local
#include "../logging/assert.hpp"
#include "../utils/print_utils.hpp"

Pathfinder::Pathfinder(const Graph* graph)
    :graph{graph}
{

}

std::vector<glm::i32vec2> Pathfinder::get_path(const glm::i32vec2 origin, const glm::i32vec2 target)
{
    return std::vector<glm::i32vec2>{};
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
