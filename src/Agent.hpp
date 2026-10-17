
#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>

struct Agent
{
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Vector2f desiredDirection;

    float preferredSpeed = 50.f;
    float sociability = 0.5f;
    float riskAversion = 0.5f;

    float detectionRadius = 35.f;
    float personalSpace = 12.f;
    float maxAcceleration = 150.f;

    void update(
        float deltaTime,
        const std::vector<Agent>& neighbours,
        float width,
        float height
    );
};
