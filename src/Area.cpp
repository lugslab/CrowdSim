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


std::size_t Area::regionIndexAtPosition(
    sf::Vector2f position
) const
{
    const float virtual_row = position.y / m_regionSize.y;
    const float row_in_bounds = std::clamp(virtual_row, 0.f, m_rows - 1.f);
   
    const float virtual_column = position.x / m_regionSize.x;
    const float column_in_bounds = std::clamp(virtual_column, 0.f, m_columns - 1.f);

    return regionIndex(
        static_cast<std::size_t>(row_in_bounds),
        static_cast<std::size_t>(column_in_bounds)
    );
}

std::vector<std::size_t> Area::getRegionAndNeighbourIndices(
    std::size_t regionIndex
) const
{
    // convert 1D index to 2D row and column
    const std::size_t column =
        regionIndex % m_columns;

    const std::size_t row =
        regionIndex / m_columns;

    // determine the bounds of the (up to) 3x3 grid around the given region
    const std::size_t minRow = row > 0 ? row - 1 : 0;
    const std::size_t maxRow = std::min(row + 1, m_rows - 1);

    const std::size_t minColumn = column > 0 ? column - 1 : 0;
    const std::size_t maxColumn = std::min(column + 1, m_columns - 1);

    // visit (up to) 9 regions and store their indices
    std::vector<std::size_t> regionIndices;
    regionIndices.reserve(9);

    for (int r = minRow; r <= maxRow; ++r)
    {
        for (int c = minColumn;
            c <= maxColumn; ++c)
        {
            regionIndices.push_back(
                r * m_columns + c
            );
        }
    }

    return regionIndices;
}
