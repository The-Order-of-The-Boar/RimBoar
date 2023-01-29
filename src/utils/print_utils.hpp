//builtin
#include <ostream>

// third party
#include <glm/vec2.hpp>


inline std::ostream& operator <<(std::ostream& ostream, const glm::i32vec2& vector)
{
    return ostream << '(' << vector.x << ',' << vector.y << ')' << std::endl;
}