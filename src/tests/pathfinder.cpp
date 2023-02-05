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

TEST_CASE("World Connections", "[Pathfinder]")
{
    RandomGenerator::setup();
    TempWorld world{};
    Graph& connection_graph = world.connection_graph;


    SECTION("Corner tiles connections")
    {
        std::array<glm::i32vec2,4> corner_points
        {
            glm::i32vec2{0,0},
            glm::i32vec2{0,world.world_size-1},
            glm::i32vec2{world.world_size-1,0},
            glm::i32vec2{world.world_size-1,world.world_size-1},
        };

        for(const auto corner_point : corner_points)
        {
            REQUIRE(connection_graph.get_node_connections(world.index_to_id(corner_point)).size() == 3);
        }
    }

    SECTION("Internal tiles connections", "[Pathfinder]")
    {
        for(size_t i = 0; i < 10; i++)
        {
            const auto tile_index = RandomGenerator::random_vec2(1, 98);

            REQUIRE(connection_graph.get_node_connections(world.index_to_id(tile_index)).size()==8);
        }
    }

    SECTION("Connections on non-empty tiles", "[Pathfinder]")
    {
        std::vector<glm::i32vec2> tiles{};
        tiles.reserve(10);
        for(size_t i = 0; i < 10; i++)
        {
            const auto tile_index = RandomGenerator::random_vec2(0, 99);
            tiles.push_back(tile_index);
            world.set_tile_empty(tile_index, false);
        }
        world.update_connections();
        for(const auto& tile : tiles)
        {
            REQUIRE(connection_graph.get_node_connections(world.index_to_id(tile)).size()==0);
        }
    }

    SECTION("Diagonal Connections", "[Pathfinder]")
    {
        for(size_t i = 0; i < 10; i++)
        {
            const glm::i32vec2 tile = RandomGenerator::random_vec2(2, 97);
            world.set_tile_empty(tile, false);
            world.update_connections();

            const std::array<glm::i32vec2,4> linear_neighbors
            {
                tile + glm::i32vec2{1,0},
                tile + glm::i32vec2{-1,0},
                tile + glm::i32vec2{0,1},
                tile + glm::i32vec2{0,-1},
            };

            for(const auto& neighbor : linear_neighbors)
            {
                REQUIRE(connection_graph.get_node_connections(world.index_to_id(neighbor)).size()==5);
            }
            
            world.set_tile_empty(tile, true);

        
        }
    }

}

TEST_CASE("World Connections Benchmark", "[Pathfinder]")
{
    RandomGenerator::setup();
    TempWorld world{};
    
    BENCHMARK("Update Connections Benchmark 1 tile")
    {
        const glm::i32vec2 tile = RandomGenerator::random_vec2(0, 99);
        world.set_tile_empty(tile, false);
        world.update_connections();
    };

    BENCHMARK("Update Connections Benchmark 100 tiles")
    {
        for(size_t i = 0; i < 100; i++)
        {
            const glm::i32vec2 tile = RandomGenerator::random_vec2(0, 99);
            world.set_tile_empty(tile, false);
        }
        world.update_connections();
    };


    BENCHMARK("Update Connections Benchmark 1000 tiles")
    {
        for(size_t i = 0; i < 1000; i++)
        {
            const glm::i32vec2 tile = RandomGenerator::random_vec2(0, 99);
            world.set_tile_empty(tile, false);
        }
        world.update_connections();
    };

}

TEST_CASE("Pathfinder Core", "[Pathfinder]")
{
    RandomGenerator::setup();
    TempWorld world{};
    Pathfinder pathfinder{&world.connection_graph, world.world_size};

    SECTION("Invalid Paths")
    {
        std::vector<glm::i32vec2> tiles{};
        tiles.reserve(10);
        for(size_t i = 0; i < 10; i++)
        {
            const auto tile_index = RandomGenerator::random_vec2(1, 99);
            tiles.push_back(tile_index);
            world.set_tile_empty(tile_index, false);
        }
        world.update_connections();
        for(const auto& tile : tiles)
        {
            const std::vector<glm::i32vec2> path = pathfinder.get_path(glm::i32vec2{0,0}, tile);
            REQUIRE(path.size() == 0);
        }
    }

    SECTION("Valid Paths")
    {
        for(size_t i = 0; i < 10; i++)
        {
            const auto tile = RandomGenerator::random_vec2(1, 99);
            const std::vector<glm::i32vec2> path = pathfinder.get_path(glm::i32vec2{0,0}, tile);
            REQUIRE(path.size() > 0);
        }
    }

    SECTION("Encircled Tiles")
    {
        for(size_t i = 0; i < 10; i++)
        {
            const auto tile = RandomGenerator::random_vec2(0, 99);

            for(int32_t x = -1; x < 2; x++)
            {

                for(int32_t y = -1; y < 2; y++)
                {
                    const glm::i32vec2 neighbor_tile = tile + glm::i32vec2{x,y};
                    if(neighbor_tile != tile && world.is_inside_boundaries(neighbor_tile))
                        world.set_tile_empty(neighbor_tile, false);
                }
            }


            for(size_t j = 0; j < 10; j++)
            {
                const glm::i32vec2 target_tile = RandomGenerator::random_vec2(0, 99);
                const std::vector<glm::i32vec2> path = pathfinder.get_path(tile, target_tile);
                REQUIRE(path.size() == 0);
            }

        }

    }
}


TEST_CASE("Pathfinding Benchmark", "[Pathfinding]")
{
    
    RandomGenerator::setup();
    TempWorld world{};
    Pathfinder pathfinder{&world.connection_graph, world.world_size};

    BENCHMARK("100 paths benchmark")
    {
        const int32_t step = world.world_size/10;
        const glm::i32vec2 origin{0,0};
        for(int32_t x = step; x < world.world_size; x+=step)
        {
            for(int32_t y = step; y < world.world_size; y+=step)
            {
                const auto path = pathfinder.get_path(origin, glm::i32vec2{x,y});
                REQUIRE(path.size() > 0);
            }

        }
    };

}