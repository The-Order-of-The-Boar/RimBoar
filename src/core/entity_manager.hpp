#pragma once

// local
#include "../slotmap/slotmap.hpp"



class Unit {

};

struct UnitID {

    SlotMap<Unit>::Key key;
};



class Wall {

};

struct WallID {

    SlotMap<Wall>::Key key;
};



class EntityManager {

private:

    SlotMap<Unit> units;
    SlotMap<Wall> walls;

public:

    Unit& get_unit(UnitID const id);
    Wall& get_wall(WallID const id);

    UnitID push_unit(Unit unit);
    WallID push_wall(Wall wall);
};