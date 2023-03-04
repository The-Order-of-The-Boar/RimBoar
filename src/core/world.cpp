// header
#include "world.hpp"

// builtin

// local
#include "../pathfinding/graph.hpp"
#include "../pathfinding/pathfinder.hpp"
#include "../utils/logging/assert.hpp"
#include "../utils/print_utils.hpp"



Map::Map(glm::u32vec2 size, std::function<Tile(glm::u32vec2)> generator):
    size_x(size.x), size_y(size.y)
{
    this->data = std::vector<std::vector<Tile>>(this->size_x, std::vector<Tile>(this->size_y));

    for (size_t x = 0; x < this->size_x; ++x)
        for (size_t y = 0; y < this->size_y; ++y)
            this->get(x, y) = generator({x, y});

    this->graph_representation = std::make_unique<Graph>(glm::u32vec2{size_x, size_y});
    this->update_graph_representation();

    this->pathfinder = std::make_unique<Pathfinder>(graph_representation.get(), size);
    this->test_path = this->pathfinder->get_path(glm::i32vec2{0, 5}, glm::u32vec2{1, 3});
}

void Map::update_tile_connections(const glm::u32vec2 index)
{

    const uint32_t tile_id = this->graph_representation->get_id_from_index(index);
    this->graph_representation->reset_node_connections(tile_id);
    for (int32_t x = -1; x < 2; x++)
    {
        for (int32_t y = -1; y < 2; y++)
        {
            const glm::i64vec2 dir{x, y};

            if (dir == glm::i64vec2{0, 0})
                continue;

            const glm::i64vec2 neighbor_pos = (glm::i64vec2)index + dir;

            if ((neighbor_pos.x < 0 || neighbor_pos.y < 0) || !this->is_inside_boundaries(neighbor_pos))
                continue;

            if (this->get(neighbor_pos).is_occupied())
                continue;

            // Diagonals movements can only happen if laterals are empty
            if (std::abs(dir.x) + std::abs(dir.y) == 2)
            {
                if (this->get((glm::i64vec2)index + glm::i64vec2{dir.x, 0}).is_occupied())
                    continue;

                if (this->get((glm::i64vec2)index + glm::i64vec2{0, dir.y}).is_occupied())
                    continue;
            }

            const uint32_t movement_cost =
                (dir.x == 0 || dir.y == 0) ? LINEAR_MOVEMENT_COST : DIAGONAL_MOVEMENT_COST;
            const uint32_t neighbor_id = this->graph_representation->get_id_from_index(neighbor_pos);
            this->graph_representation->add_node_connection(tile_id, neighbor_id, movement_cost);
        }
    }
}

void Map::update_graph_representation()
{
    for (size_t x = 0; x < this->size_x; x++)
    {
        for (size_t y = 0; y < this->size_y; y++)
        {
            this->update_tile_connections(glm::i32vec2{x, y});
        }
    }
}

glm::u32vec2 Map::size() const
{
    return {this->size_x, this->size_y};
}

bool Map::is_inside_boundaries(const glm::u32vec2 index) const
{
    if (index.x >= this->size_x)
        return false;
    if (index.y >= this->size_y)
        return false;

    return true;
}

Tile& Map::get(glm::u32vec2 position)
{
    return this->get(position.x, position.y);
}

Tile const& Map::get(glm::u32vec2 position) const
{
    return this->get(position.x, position.y);
}

Tile& Map::get(size_t x, size_t y)
{
    rb_assert(x < this->size_x);
    rb_assert(y < this->size_y);

    return this->data[x][y];
}

Tile const& Map::get(size_t x, size_t y) const
{
    auto& map = const_cast<Map&>(*this);
    return const_cast<Tile const&>(map.get(x, y));
}



std::optional<float> Tile::walk_speed() const
{
    if (this->is_occupied())
        return std::nullopt;

    return {1.0};
}

bool Tile::is_occupied() const
{
    return (this->unit.has_value() || this->wall.has_value());
}



void World::push_unit(Unit unit, glm::u32vec2 position)
{
    auto& tile = this->map.get(position);

    rb_assert(tile.is_occupied() == false);
    rb_assert(tile.unit.has_value() == false);

    auto id = this->entities.push_unit(std::move(unit));
    tile.unit = id;

    rb_assert(tile.is_occupied());

    this->map.update_graph_representation();
}

void World::push_wall(Wall wall, glm::u32vec2 position)
{
    auto& tile = this->map.get(position);

    rb_assert(tile.is_occupied() == false);
    rb_assert(tile.wall.has_value() == false);

    auto id = this->entities.push_wall(std::move(wall));
    tile.wall = id;

    rb_assert(tile.is_occupied());

    this->map.update_graph_representation();
}

void World::remove_unit(glm::u32vec2 position)
{
    auto& tile = this->map.get(position);

    rb_assert(tile.unit.has_value());
    tile.unit = std::nullopt;

    this->map.update_graph_representation();
}

void World::remove_wall(glm::u32vec2 position)
{
    auto& tile = this->map.get(position);

    rb_assert(tile.wall.has_value());
    tile.wall = std::nullopt;

    this->map.update_graph_representation();
}
