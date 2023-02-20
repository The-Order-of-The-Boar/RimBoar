// header
#include "world.hpp"

// builtin

// local
#include "../logging/assert.hpp"
#include "../data_structures/graph.hpp"
#include "../pathfinding/pathfinder.hpp"
#include <glm/ext/vector_int2_sized.hpp>




Map::Map(glm::u32vec2 size, std::function<Tile(glm::u32vec2)> generator): size_x(size.x), size_y(size.y)
{
    this->data = std::vector<std::vector<Tile>>(this->size_x, std::vector<Tile>(this->size_y));

    for (size_t x = 0; x < this->size_x; ++x)
        for (size_t y = 0; y < this->size_y; ++y)
            this->get(x, y) = generator({x, y});
    

    this->graph_representation = std::make_unique<Graph>(size_x * size_y);
    this->update_graph_representation();

    this->pathfinder = std::make_unique<Pathfinder>(graph_representation.get(),size);
    this->test_path = this->pathfinder->get_path(glm::i32vec2{0,5}, glm::i32vec2{1,3});

}

void Map::update_tile_connections(const glm::i32vec2 index)
{
    auto index_to_id = [this](const glm::i32vec2 index) -> int32_t
    {
        return index.x*this->size_x + index.y;
    };

    const int32_t tile_id = index_to_id(index);
    this->graph_representation->reset_node_connections(tile_id);
    for(int32_t x = -1; x < 2; x++)
    {
        for(int32_t y = -1; y < 2; y++)
        {
            const glm::i32vec2 dir{x,y};
            const glm::i32vec2 neighbor_pos = index + dir;
            
            if(!this->is_inside_boundaries(neighbor_pos) || neighbor_pos == index) continue;

            const Tile& neighbor = this->get(neighbor_pos);

            const std::optional<float> walk_speed = neighbor.walk_speed();
            if(!walk_speed.has_value()) continue;
            
            // Diagonals movements can only happen if laterals are empty
            if(abs(dir.x) + abs(dir.y) == 2)
            {
                if(!this->get(index + glm::i32vec2{dir.x,0}).walk_speed().has_value()) continue;
                if(!this->get(index + glm::i32vec2{0,dir.y}).walk_speed().has_value()) continue;
            }


            const int32_t movement_cost = (dir.x == 0 || dir.y == 0) ? LINEAR_MOVEMENT_COST : DIAGONAL_MOVEMENT_COST;
            const int32_t neighbor_id = index_to_id(neighbor_pos);
            this->graph_representation->add_node_connection(tile_id, neighbor_id, movement_cost);
        }
    }

}

void Map::update_graph_representation()
{
    for(size_t x = 0; x < this->size_x; x++)
    {
        for(size_t y = 0; y < this->size_y; y++)
        {
            this->update_tile_connections(glm::i32vec2{x,y});
        }
    }
}

glm::u32vec2 Map::size() const
{
    return {this->size_x, this->size_y};
}

bool Map::is_inside_boundaries(const glm::i32vec2 index) const
{
    if(index.x < 0 || index.x >= this->size_x) return false;
    if(index.y < 0 || index.y >= this->size_y) return false;

    return true;
}

Tile& Map::get(glm::u32vec2 position) {

    return this->get(position.x, position.y);
}

Tile const& Map::get(glm::u32vec2 position) const {

    return this->get(position.x, position.y);
}

Tile& Map::get(size_t x, size_t y) {

    rb_assert(x < this->size_x);
    rb_assert(y < this->size_y);

    return this->data[x][y];
}

Tile const& Map::get(size_t x, size_t y) const {

    auto& map = const_cast<Map&>(*this);
    return const_cast<Tile const&>(map.get(x, y));
}

std::optional<float> Tile::walk_speed() const {

    if (this->state != State::Emtpy)
        return std::nullopt;
    
    return {1.0};
}
