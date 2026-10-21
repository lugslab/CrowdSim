#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace VectorUtils
{
    inline sf::Vector2f normalise(sf::Vector2f v)
    {
        // SFML has a `Vector2::normalize()` function, but it does not handle zero-length vectors
        float magnitude = v.length();

        if (magnitude < 0.0001f)
            return { 0.f, 0.f };

        return v / magnitude;
    }

    inline bool isZero(sf::Vector2f v)
    {
        constexpr float epsilon = 0.0001f;
        return v.lengthSquared() < epsilon * epsilon;
    }
}
