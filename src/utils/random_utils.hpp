//builtin
#include <cstdint>
#include <glm/ext/vector_int2_sized.hpp>
#include <time.h>
#include <stdlib.h>

//third party
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
        return min + rand()%(max+1-min);
    }

    static glm::i32vec2 random_vec2(const int32_t min, const int32_t max)
    {
        const auto x = RandomGenerator::random_int(min, max);
        const auto y = RandomGenerator::random_int(min, max);
        return glm::i32vec2{x,y};
    }
};