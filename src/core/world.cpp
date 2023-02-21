// header
#include "world.hpp"

// local
#include "../logging/assert.hpp"



Map::Map(glm::u32vec2 size, std::function<Tile(glm::u32vec2)> generator):
    size_x(size.x), size_y(size.y)
{
    this->data = std::vector<std::vector<Tile>>(this->size_x, std::vector<Tile>(this->size_y));

    for (size_t x = 0; x < this->size_x; ++x)
        for (size_t y = 0; y < this->size_y; ++y)
            this->get(x, y) = generator({x, y});
}

glm::u32vec2 Map::size() const
{
    return {this->size_x, this->size_y};
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



void World::push_unit(Unit unit, glm::u32vec2 position) {

    auto& tile = this->map.get(position);

    rb_assert(tile.is_occupied() == false);
    rb_assert(tile.unit.has_value() == false);

    auto id = this->entities.push_unit(std::move(unit));
    tile.unit = id;
}

void World::push_wall(Wall wall, glm::u32vec2 position) {

    auto& tile = this->map.get(position);

    rb_assert(tile.is_occupied() == false);
    rb_assert(tile.wall.has_value() == false);

    auto id = this->entities.push_wall(std::move(wall));
    tile.wall = id;
}
