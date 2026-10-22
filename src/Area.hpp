#pragma once

#include <cstddef>
#include <vector>

#include <SFML/System/Vector2.hpp>


// Add to Area's public section:



class Area {
public:
    Area(sf::Vector2f areaSize,
        std::size_t rows,
        std::size_t columns);

    float getWidth() const {
        return m_areaSize.x;
    }

    float getHeight() const {
        return m_areaSize.y;
    }

    void moveWithinBounds(sf::Vector2f& position, float radius) const;

    std::vector<sf::Vector2f> placeDestinations(float offset) const;

private:
    const sf::Vector2f m_areaSize;
    const std::size_t m_rows;
    const std::size_t m_columns;
    const sf::Vector2f m_regionSize;

};