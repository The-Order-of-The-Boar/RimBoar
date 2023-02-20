//builtin
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <glm/ext/vector_int2_sized.hpp>
#include <time.h>
#include <stdlib.h>
#include <vector>

//third party
#include <catch.hpp>
#include <glm/vec2.hpp>

//local
#include "../pathfinding/pathfinder.hpp"
#include "../utils/time_utils.hpp"
#include "../utils/random_utils.hpp"
#include "../core/world.hpp"

Map create_map(glm::u32vec2 size) {

    auto output = Map{size, [&](glm::u32vec2){ return Tile{.state = Tile::State::Emtpy }; }};
    return output;
}

std::unique_ptr<World> create_world(const glm::i32vec2 size)
{
    return std::unique_ptr<World>{new World{Map{size, [&](glm::u32vec2){ return Tile{.state = Tile::State::Emtpy }; }}}};
}

TEST_CASE("Pathfinding Benchmark", "[!benchmark]")
{ 
    RandomGenerator::setup();
    const glm::i32vec2 size{100,100};
    auto world = create_world(size);
    Pathfinder pathfinder{world->map.graph_representation.get(), size};

    
    BENCHMARK("Corner to corner benchmark")
    {
        const glm::i32vec2 origin{0,0};
        const glm::i32vec2 target = size - glm::i32vec2{1,1};
        const auto path = pathfinder.get_path(origin, target);
        REQUIRE(path.size() == size.x-1);
    };


    BENCHMARK("100 paths benchmark")
    {
        const glm::i32vec2 origin{0,0};
        for(int32_t x = 10; x < size.x; x+=10)
        {
            for(int32_t y = 10; y < size.y; y+=10)
            {
                const auto path = pathfinder.get_path(origin, glm::i32vec2{x,y});
                REQUIRE(path.size() > 0);
            }
        }
    };
    
    BENCHMARK("No path benchmark")
    {
        const glm::i32vec2 origin{0,0};
        const glm::i32vec2 target = size/2;
        world->map.get(target).state = Tile::State::Wall;
        world->map.update_graph_representation();
        const auto path = pathfinder.get_path(origin, target);
        REQUIRE(path.size() == 0);
    };

}