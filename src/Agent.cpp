
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
    const Personality& p = personality;

    // 1. Goal seeking.
    sf::Vector2f toDestination =
        destination - position;

    float distanceToDestination =
        length(toDestination);

    constexpr float slowingRadius = 50.f;

    float desiredSpeed = p.preferredSpeed;

    if (distanceToDestination < slowingRadius)
    {
        desiredSpeed *=
            distanceToDestination / slowingRadius;
    }

    sf::Vector2f goalVelocity =
        normalize(toDestination) *
        desiredSpeed * p.goalWeight;

    // 2. Separation.
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
            distance > p.detectionRadius)
            continue;

        float strength =
            (p.detectionRadius - distance) /
            p.detectionRadius;

        if (distance < p.personalSpace)
            strength *= 3.f;

        avoidance +=
            normalize(difference) * strength;
    }

    sf::Vector2f targetVelocity = goalVelocity;

    if (length(avoidance) > 0.0001f)
    {
        targetVelocity +=
            normalize(avoidance) *
            p.preferredSpeed * p.avoidanceWeight;
    }

    // 3. Social attraction.
    if (companionIndex >= 0 &&
        static_cast<std::size_t>(companionIndex) <
        neighbours.size())
    {
        const Agent& companion =
            neighbours[companionIndex];

        sf::Vector2f toCompanion =
            companion.position - position;

        float distance = length(toCompanion);

        // Follow only when sufficiently separated.
        if (distance > 45.f)
        {
            targetVelocity +=
                normalize(toCompanion) *
                p.preferredSpeed * p.socialWeight;
        }
    }

    // 4. Steering acceleration.
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

    float maxSpeed = p.preferredSpeed * 1.5f;

    if (speed > maxSpeed)
    {
        velocity = normalize(velocity) * maxSpeed;
    }

    // 5. Movement.
    position += velocity * deltaTime;

    constexpr float radius = 4.f;

    position.x = std::clamp(
        position.x, radius, width - radius
    );

    position.y = std::clamp(
        position.y, radius, height - radius
    );
}
