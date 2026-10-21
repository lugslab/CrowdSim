#pragma once

#include <vector>

#include <SFML/System/Vector2.hpp>

class Area {
public:
    // Constructor
    Area();

    static unsigned getWidth() {
        return size.x;
    }

    static unsigned getHeight() {
        return size.y;
    }

    static void moveWithinBounds(sf::Vector2f& position, float radius);

    static std::vector<sf::Vector2f> placeDestinations(float offset);
private:
    static constexpr sf::Vector2u size{ 1000, 700 };
};