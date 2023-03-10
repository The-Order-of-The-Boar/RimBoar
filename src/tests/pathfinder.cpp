// builtin
#include <catch2/benchmark/catch_benchmark.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <glm/ext/vector_int2_sized.hpp>
#include <stdlib.h>
#include <time.h>
#include <vector>

// third party
#include <catch.hpp>
#include <glm/vec2.hpp>

// local
#include "../core/world.hpp"
#include "../pathfinding/pathfinder.hpp"
#include "../utils/random_utils.hpp"
#include "../utils/time_utils.hpp"

Map create_map(glm::u32vec2 size)
{
    auto output = Map{size, [&](glm::u32vec2) { return Tile{}; }};
    return output;
}

std::unique_ptr<World> create_world(const glm::i32vec2 size)
{
    return std::unique_ptr<World>{
        new World{.map = Map{size, [&](glm::u32vec2) { return Tile{}; }}}};
}

TEST_CASE("Pathfinding Benchmark", "[!benchmark]")
{
    RandomGenerator::setup();
    const glm::i32vec2 size{100, 100};
    auto world = create_world(size);
    Pathfinder pathfinder{world->map.graph_representation.get(), size};


    BENCHMARK("Corner to corner benchmark")
    {
        const glm::i32vec2 origin{0, 0};
        const glm::i32vec2 target = size - glm::i32vec2{1, 1};
        auto const path = pathfinder.get_path(origin, target);
        REQUIRE(path.value().size() == size.x - 1);
    };


    BENCHMARK("100 paths benchmark")
    {
        const glm::i32vec2 origin{0, 0};
        for (int32_t x = 10; x < size.x; x += 10)
        {
            for (int32_t y = 10; y < size.y; y += 10)
            {
                auto const path = pathfinder.get_path(origin, glm::i32vec2{x, y});
                REQUIRE(path.value().size() > 0);
            }
        }
    };

    BENCHMARK("No path benchmark")
    {
        const glm::i32vec2 origin{0, 0};
        const glm::i32vec2 target = size / 2;
        world->push_wall(Wall{}, target);
        world->map.update_graph_representation();
        auto const path = pathfinder.get_path(origin, target);
        REQUIRE(path.has_value() == false);
    };
}