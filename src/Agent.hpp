
#pragma once

#include "Personality.hpp"

#include <SFML/System/Vector2.hpp>

#include <cstddef>
#include <vector>

struct Agent
{
    sf::Vector2f position{};
    sf::Vector2f velocity{};
    sf::Vector2f destination{};

    Personality personality{};

    // Index of a companion in the simulation.
    // -1 means no companion.
    int companionIndex = -1;

    float maxAcceleration = 150.f;

    void update(
        float deltaTime,
        const std::vector<Agent>& neighbours,
        std::size_t selfIndex,
        float width,
        float height
    );

    bool hasReachedDestination() const;
};
