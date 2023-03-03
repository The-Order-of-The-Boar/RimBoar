// header
#include "entity_manager.hpp"



Unit& EntityManager::get_unit(UnitID const id)
{
    auto unit = this->units.get(id.key);
    rb_assert(unit.has_value());
    return *unit;
}

Wall& EntityManager::get_wall(WallID const id)
{
    auto wall = this->walls.get(id.key);
    rb_assert(wall.has_value());
    return *wall;
}


UnitID EntityManager::push_unit(Unit unit)
{
    auto key = this->units.push(std::move(unit));
    return UnitID{.key = key};
}

WallID EntityManager::push_wall(Wall wall)
{
    auto key = this->walls.push(std::move(wall));
    return WallID{.key = key};
}
