
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
    const vector<Agent>& neighbours,
    float width,
    float height,
    size_t& neighbourChecks,
    size_t& neighbourInteractions
)
{
    // Identify competing influences.
    
    // Current goal.
    sf::Vector2f goalVelocity = calculateGoalVelocity();

    // Separation.
    sf::Vector2f proximityAvoidance = calculateAvoidanceVelocity(neighbours, neighbourChecks, neighbourInteractions);
    
    // Companion attraction.
    sf::Vector2f socialAttraction = calculateAttractionVelocity(neighbours);

    // Resolve and apply velocities
    sf::Vector2f targetVelocity = resolveTargetVelocity(goalVelocity, proximityAvoidance, socialAttraction);

    applyAcceleration(targetVelocity, deltaTime);

    // Limit maximum speed.
    clampSpeed();

    move(width, height, deltaTime);
}

sf::Vector2f Agent::calculateGoalVelocity() const {
    // 1. Goal seeking.
    sf::Vector2f toDestination =
        destination - position;

    float distanceToDestination =
        toDestination.length();

    constexpr float slowingRadius = 50.f;

    float desiredSpeed = personality.preferredSpeed;

    if (distanceToDestination < slowingRadius)
    {
        desiredSpeed *=
            distanceToDestination / slowingRadius;
    }

    sf::Vector2f goalVelocity =
        VectorUtils::normalise(toDestination) *
        desiredSpeed * personality.goalWeight;

    return goalVelocity;
}

sf::Vector2f Agent::calculateAvoidanceVelocity(const vector<Agent>& neighbours, size_t& neighbourChecks, size_t& neighbourInteractions) {
    sf::Vector2f combinedAvoidance{ 0.f, 0.f };

    for (size_t i = 0; i < neighbours.size(); ++i)
    {
        // count the number of neighbour checks for performance analysis
        ++neighbourChecks;

        const Agent& other = neighbours[i];

        sf::Vector2f difference =
            position - other.position;

        float distance = difference.length();
        // ignore if too close (including self) or too far away
        if (distance < 0.0001f ||
            distance > personality.detectionRadius)
            continue;

        // count the number of neighbour interactions for performance analysis
        ++neighbourInteractions;
        // determine the strength of avoidance based on distance and personal space
        float strength =
            (personality.detectionRadius - distance) /
            personality.detectionRadius;

        if (distance < personality.personalSpace)
            strength *= 3.f;

        // combine avoidance vectors from all neighbours
        combinedAvoidance +=
            VectorUtils::normalise(difference) * strength;
    }

    // weight response by personality and preferred speed
    sf::Vector2f avoidanceVelocity{ 0.f, 0.f };
    
    if (combinedAvoidance.length() > 0.0001f)
    {
        avoidanceVelocity =
            VectorUtils::normalise(combinedAvoidance) *
            personality.preferredSpeed * personality.avoidanceWeight;
    }

    return avoidanceVelocity;
}

sf::Vector2f Agent::calculateAttractionVelocity(const vector<Agent>& neighbours) {
    sf::Vector2f attractionVelocity{ 0.f, 0.f };

    if (companionIndex >= 0 &&
        static_cast<size_t>(companionIndex) <
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
            // weight response by personality and preferred speed
            attractionVelocity =
                VectorUtils::normalise(toCompanion) *
                personality.preferredSpeed * personality.socialWeight;
        }
    }
    return attractionVelocity;
}

sf::Vector2f Agent::resolveTargetVelocity(sf::Vector2f goalVelocity, sf::Vector2f proximityAvoidance, sf::Vector2f socialAttraction) {
    // combine competiting influencess
    return  goalVelocity + proximityAvoidance + socialAttraction;
}

void Agent::applyAcceleration(sf::Vector2f targetVelocity, float deltaTime) {
    // calculate, clamp, and apply acceleration
    sf::Vector2f acceleration =
        targetVelocity - velocity;

    // clamp to maximum acceleration
    float accelerationLength = acceleration.length();

    if (accelerationLength > maxAcceleration)
    {
        acceleration =
            VectorUtils::normalise(acceleration) * maxAcceleration;
    }
    // update velocity based on acceleration over deltaTime
    velocity += acceleration * deltaTime;
}

void Agent::clampSpeed() {
    // Limit maximum speed to 1.5 times the preferred speed.
    float speed = velocity.length();
    float maxSpeed = personality.preferredSpeed * 1.5f;
    if (speed > maxSpeed)
    {
        velocity = VectorUtils::normalise(velocity) * maxSpeed;
    }
}

void Agent::move(float width, float height, float deltaTime) {
    // update position after deltaTime seconds
    position += velocity * deltaTime;
    // limit movement to within the bounds of the simulation area
    constexpr float radius = 4.f;

    position.x = clamp(
        position.x, radius, width - radius
    );

    position.y = clamp(
        position.y, radius, height - radius
    );
}