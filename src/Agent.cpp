
#include "Agent.hpp"

#include <algorithm>
#include <cmath>

namespace
{
    float length(sf::Vector2f v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    sf::Vector2f normalize(sf::Vector2f v)
    {
        float magnitude = length(v);

        if (magnitude < 0.0001f)
            return { 0.f, 0.f };

        return v / magnitude;
    }
}

void Agent::update(
    float deltaTime,
    const std::vector<Agent>& neighbours,
    float width,
    float height
)
{
    sf::Vector2f avoidance{ 0.f, 0.f };

    for (const auto& other : neighbours)
    {
        sf::Vector2f difference =
            position - other.position;

        float distance = length(difference);

        if (distance < 0.0001f ||
            distance > detectionRadius)
            continue;

        // Stronger repulsion at shorter distances.
        float strength =
            (detectionRadius - distance) /
            detectionRadius;

        if (distance < personalSpace)
            strength *= 3.f;

        avoidance += normalize(difference) * strength;
    }

    sf::Vector2f preferredVelocity =
        normalize(desiredDirection) * preferredSpeed;

    sf::Vector2f targetVelocity = preferredVelocity;

    if (length(avoidance) > 0.0001f)
    {
        targetVelocity +=
            normalize(avoidance) *
            preferredSpeed * 1.5f;
    }

    // Steer towards the target velocity.
    sf::Vector2f acceleration =
        targetVelocity - velocity;

    float accelerationLength = length(acceleration);

    if (accelerationLength > maxAcceleration)
    {
        acceleration =
            normalize(acceleration) * maxAcceleration;
    }

    velocity += acceleration * deltaTime;

    // Limit maximum speed.
    float speed = length(velocity);

    if (speed > preferredSpeed * 1.5f)
    {
        velocity =
            normalize(velocity) *
            preferredSpeed * 1.5f;
    }

    position += velocity * deltaTime;

    constexpr float radius = 4.f;

    // Reflect direction at the boundaries.
    if (position.x < radius ||
        position.x > width - radius)
    {
        velocity.x = -velocity.x;
        desiredDirection.x = -desiredDirection.x;
        position.x = std::clamp(
            position.x, radius, width - radius
        );
    }

    if (position.y < radius ||
        position.y > height - radius)
    {
        velocity.y = -velocity.y;
        desiredDirection.y = -desiredDirection.y;
        position.y = std::clamp(
            position.y, radius, height - radius
        );
    }
}
