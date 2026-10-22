#include "Area.hpp"

#include <algorithm>
#include <stdexcept>
#include <vector>

Area::Area(sf::Vector2f areaSize, std::size_t rows, std::size_t columns)
    : m_areaSize(areaSize)
    , m_rows(std::max(rows, std::size_t{ 1 }))
    , m_columns(std::max(columns, std::size_t{ 1 }))
    , m_regionSize(
        areaSize.x / static_cast<float>(m_columns),
        areaSize.y / static_cast<float>(m_rows)
    )
{
}

void Area::moveWithinBounds(sf::Vector2f& position, float radius) const {
    // Ensure the position is within the bounds of the area, considering the radius.
    position.x = std::clamp(position.x, radius, m_areaSize.x - radius);
    position.y = std::clamp(position.y, radius, m_areaSize.y - radius);
}


std::vector<sf::Vector2f> Area::placeDestinations(float offset) const
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

