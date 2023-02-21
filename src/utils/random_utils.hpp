#pragma once

// builtin
#include <cstdint>
#include <glm/ext/vector_int2_sized.hpp>
#include <stdlib.h>
#include <time.h>

// third party
#include <glm/vec2.hpp>

class RandomGenerator
{
public:

    static void setup()
    {
        srand(time(nullptr));
    }

    static int32_t random_int(const int32_t min, const int32_t max)
    {
        return min + rand() % (max + 1 - min);
    }

    static glm::i32vec2 random_vec2(const int32_t min, const int32_t max)
    {
        auto const x = RandomGenerator::random_int(min, max);
        auto const y = RandomGenerator::random_int(min, max);
        return glm::i32vec2{x, y};
    }
};