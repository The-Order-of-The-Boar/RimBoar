#pragma once

#include <ostream>
#include <cstdint>

namespace boarglib
{

template <class NumberT>
class Vector2
{
public:
    NumberT x;
    NumberT y;

    constexpr Vector2(const NumberT x = 0, const NumberT y = 0)
        :x{x}, y{y} 
    {

    }

    inline Vector2<NumberT> operator+(const Vector2<NumberT> other) const
    {
        return Vector2<NumberT>
        {
            this->x + other.x,
            this->y + other.y
        };
    }

    inline Vector2<NumberT> operator-(const Vector2<NumberT> other) const
    {
        return Vector2<NumberT>
        {
            this->x - other.x,
            this->y - other.y
        };
    }

    inline Vector2<NumberT> operator*(const Vector2<NumberT> other) const
    {
        return Vector2<NumberT>
        {
            this->x * other.x,
            this->y * other.y
        };
    }

    inline Vector2<NumberT> operator/(const Vector2<NumberT> other) const
    {
        return Vector2<NumberT>
        {
            this->x / other.x,
            this->y / other.y
        };
    }

    inline bool operator==(const Vector2<NumberT> other) const
    {
        return this->x == other.x && this->y == other.y;
    }

    inline bool operator!=(const Vector2<NumberT> other) const
    {
        return this->x != other.x || this->y != other.y;
    }
};

using Vector2i32 = Vector2<int32_t>;
using Vector2f   = Vector2<float>;
using Vector2d   = Vector2<double>;

};

template<class NumberT>
inline std::ostream& operator <<(std::ostream& ostream, const boarglib::Vector2<NumberT>& vector)
{
    return ostream << '(' << vector.x << ',' << vector.y << ")\n";
}