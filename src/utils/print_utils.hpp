#pragma once

// builtin
#include <ostream>

// third party
#include <glm/vec2.hpp>


inline std::ostream& operator<<(std::ostream& ostream, glm::i32vec2 const& vector)
{
    return ostream << '(' << vector.x << ',' << vector.y << ')' << std::endl;
}