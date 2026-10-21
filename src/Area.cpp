#include "Area.hpp"

#include <algorithm>

void Area::moveWithinBounds(sf::Vector2f& position, float radius) {
    // Ensure the position is within the bounds of the area, considering the radius.
    position.x = std::clamp(position.x, radius, static_cast<float>(size.x) - radius);
    position.y = std::clamp(position.y, radius, static_cast<float>(size.y) - radius);
}


std::vector<sf::Vector2f> Area::placeDestinations(float offset)
{
    const float top = offset;
    const float bottom = getHeight() - offset;
    const float left = offset;
    const float right = getWidth() - offset;

    return {
        {left, top},
        {left, bottom},
        {right, top},
        {right, bottom},
        // centre
        {getWidth() / 2.f, getHeight() / 2.f}
    };
}