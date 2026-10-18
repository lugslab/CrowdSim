
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

bool Agent::hasReachedDestination() const
{
    return length(destination - position) < 12.f;
}

void Agent::update(
    float deltaTime,
    const std::vector<Agent>& neighbours,
    std::size_t selfIndex,
    float width,
    float height
)
{
    // 1. Seek the destination.
    sf::Vector2f toDestination =
        destination - position;

    float distanceToDestination =
        length(toDestination);

    float desiredSpeed = preferredSpeed;

    // Slow down near the destination.
    constexpr float slowingRadius = 50.f;

    if (distanceToDestination < slowingRadius)
    {
        desiredSpeed *=
            distanceToDestination / slowingRadius;
    }

    sf::Vector2f desiredVelocity =
        normalize(toDestination) * desiredSpeed;

    // 2. Calculate separation.
    sf::Vector2f avoidance{ 0.f, 0.f };

    for (std::size_t i = 0; i < neighbours.size(); ++i)
    {
        if (i == selfIndex)
            continue;

        const Agent& other = neighbours[i];

        sf::Vector2f difference =
            position - other.position;

        float distance = length(difference);

        if (distance < 0.0001f ||
            distance > detectionRadius)
            continue;

        float strength =
            (detectionRadius - distance) /
            detectionRadius;

        if (distance < personalSpace)
            strength *= 3.f;

        avoidance +=
            normalize(difference) * strength;
    }

    // 3. Combine seeking and avoidance.
    sf::Vector2f targetVelocity = desiredVelocity;

    if (length(avoidance) > 0.0001f)
    {
        targetVelocity +=
            normalize(avoidance) *
            preferredSpeed * 1.5f;
    }

    // 4. Apply limited acceleration.
    sf::Vector2f acceleration =
        targetVelocity - velocity;

    float accelerationLength = length(acceleration);

    if (accelerationLength > maxAcceleration)
    {
        acceleration =
            normalize(acceleration) * maxAcceleration;
    }

    velocity += acceleration * deltaTime;

    // Limit speed.
    float speed = length(velocity);

    if (speed > preferredSpeed * 1.5f)
    {
        velocity =
            normalize(velocity) *
            preferredSpeed * 1.5f;
    }

    // 5. Update position.
    position += velocity * deltaTime;

    constexpr float radius = 4.f;

    // Keep agents inside the simulation area.
    position.x = std::clamp(
        position.x, radius, width - radius
    );

    position.y = std::clamp(
        position.y, radius, height - radius
    );
}
