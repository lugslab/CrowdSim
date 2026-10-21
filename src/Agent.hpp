#pragma once

#include "Personality.hpp"

#include <SFML/System/Vector2.hpp>

#include <cstddef>
#include <vector>

using namespace std;

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
        const vector<Agent>& neighbours,
        float width,
        float height,
        size_t& neighbourChecks,
        size_t& neighbourInteractions
    );
    sf::Vector2f Agent::calculateGoalVelocity() const;
    sf::Vector2f Agent::calculateAvoidanceVelocity(const vector<Agent>& neighbours, size_t& neighbourChecks, size_t& neighbourInteractions);
    sf::Vector2f Agent::calculateAttractionVelocity(const vector<Agent>& neighbours);
    sf::Vector2f Agent::resolveTargetVelocity(sf::Vector2f goalVelocity, sf::Vector2f proximityAvoidance, sf::Vector2f socialAttraction);
    void Agent::applyAcceleration(sf::Vector2f targetVelocity, float deltaTime);
    void Agent::clampSpeed();
    void move(float width, float height, float deltaTime);

    bool hasReachedDestination() const;
};
