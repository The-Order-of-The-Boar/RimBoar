//builtin
#include <cstddef>
#include <cstdint>
#include <queue>
#include <iostream>

//third party
#include <glm/vec2.hpp>
#include <glm/ext/scalar_int_sized.hpp>
#include <glm/ext/vector_int2_sized.hpp>

//local
#include "../data_structures/graph.hpp"


class Pathfinder
{
private:
    const Graph* graph;


public:
    Pathfinder(const Graph* graph);

    std::vector<glm::i32vec2> get_path(const glm::i32vec2 origin, glm::i32vec2 target);

};




//TEMPORARY CODE TO TEST PATHFINDING
    struct TempTile
    {
        bool empty = true;
        glm::i32vec2 index;
        int32_t id;
    };

    class TempWorld
    {
    private:
        const int linear_move_cost = 10;
        const int diagonal_move_cost = 14;

    public:
        std::array<std::array<TempTile,100>,100> world_tiles;
        const int32_t world_size;
        Graph connection_graph{static_cast<int32_t>(world_size*world_size)};

    public:
        TempWorld();
        void update_tile_connections(const TempTile* tile);
        void update_connections();
        void set_tile_empty(const glm::i32vec2 index, const bool empty);


        bool is_inside_boundaries(const glm::i32vec2 index) const;
        TempTile* get_tile(const glm::i32vec2 index);

        int32_t index_to_id(const glm::i32vec2 id) const;
        glm::i32vec2 id_to_index(const int32_t id) const;

    };
//TEMPORARY CODE TO TEST PATHFINDING