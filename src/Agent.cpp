
#include "Agent.hpp"
#include "VectorUtils.hpp"

#include <algorithm>
#include <cmath>

bool Agent::hasReachedDestination() const
{
    return (destination - position).length() < 12.f;
}

void Agent::update(
    float deltaTime,
    const std::vector<Agent>& neighbours,
    std::size_t selfIndex,
    float width,
    float height,
    std::size_t& neighbourChecks,
    std::size_t& neighbourInteractions
)
{
    const Personality& p = personality;

    // 1. Goal seeking.
    sf::Vector2f toDestination =
        destination - position;

    float distanceToDestination =
        toDestination.length();

    constexpr float slowingRadius = 50.f;

    float desiredSpeed = p.preferredSpeed;

    if (distanceToDestination < slowingRadius)
    {
        desiredSpeed *=
            distanceToDestination / slowingRadius;
    }

    sf::Vector2f goalVelocity =
        VectorUtils::normalise(toDestination) *
        desiredSpeed * p.goalWeight;

    // 2. Separation.
    sf::Vector2f avoidance{ 0.f, 0.f };

    for (std::size_t i = 0; i < neighbours.size(); ++i)
    {
        if (i == selfIndex)
            continue;

        ++neighbourChecks;

        const Agent& other = neighbours[i];

        sf::Vector2f difference =
            position - other.position;

        float distance = difference.length();

        if (distance < 0.0001f ||
            distance > p.detectionRadius)
            continue;

        ++neighbourInteractions;

        float strength =
            (p.detectionRadius - distance) /
            p.detectionRadius;

        if (distance < p.personalSpace)
            strength *= 3.f;

        avoidance +=
            VectorUtils::normalise(difference) * strength;
    }

    sf::Vector2f targetVelocity = goalVelocity;

    if (avoidance.length() > 0.0001f)
    {
        targetVelocity +=
            VectorUtils::normalise(avoidance) *
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

        float distance = toCompanion.length();

        // Follow only when sufficiently separated.
        if (distance > 45.f)
        {
            targetVelocity +=
                VectorUtils::normalise(toCompanion) *
                p.preferredSpeed * p.socialWeight;
        }
    }

    // 4. Steering acceleration.
    sf::Vector2f acceleration =
        targetVelocity - velocity;

    float accelerationLength = acceleration.length();

    if (accelerationLength > maxAcceleration)
    {
        acceleration =
            VectorUtils::normalise(acceleration) * maxAcceleration;
    }

    velocity += acceleration * deltaTime;

    // Limit maximum speed.
    float speed = velocity.length();

    float maxSpeed = p.preferredSpeed * 1.5f;

    if (speed > maxSpeed)
    {
        velocity = VectorUtils::normalise(velocity) * maxSpeed;
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
