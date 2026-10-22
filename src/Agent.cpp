#include "Agent.hpp"
#include "Area.hpp"
#include "VectorUtils.hpp"

#include <algorithm>

bool Agent::hasReachedDestination() const
{
    return (destination - position).length() < destinationTolerance;
}

void Agent::update(
    const Area& area,
    float deltaTime,
    const std::vector<Agent>& neighbours,
    std::size_t& neighbourChecks,
    std::size_t& neighbourInteractions
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

    move(area, deltaTime);
}

sf::Vector2f Agent::calculateGoalVelocity() const {
    // 1. Goal seeking.
    sf::Vector2f toDestination =
        destination - position;

    float distanceToDestination =
        toDestination.length();

    float desiredSpeed = personality.preferredSpeed;

    if (distanceToDestination < goalApproachRadius)
    {
        // slow down as we approach the destination.
        desiredSpeed *=
            distanceToDestination / goalApproachRadius;
    }

    sf::Vector2f goalVelocity =
        VectorUtils::normalise(toDestination) *
        desiredSpeed * personality.goalWeight;

    return goalVelocity;
}

sf::Vector2f Agent::calculateAvoidanceVelocity(const std::vector<Agent>& neighbours, std::size_t& neighbourChecks, std::size_t& neighbourInteractions) {
    sf::Vector2f combinedAvoidance{ 0.f, 0.f };

    for (std::size_t i = 0; i < neighbours.size(); ++i)
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
            strength *= personalSpaceMultiplier;

        // combine avoidance vectors from all neighbours
        combinedAvoidance +=
            VectorUtils::normalise(difference) * strength;
    }

    // weight response by personality and preferred speed
    sf::Vector2f avoidanceVelocity{ 0.f, 0.f };
    
    if (!VectorUtils::isZero(combinedAvoidance))
    {
        avoidanceVelocity =
            VectorUtils::normalise(combinedAvoidance) *
            personality.preferredSpeed * personality.avoidanceWeight;
    }

    return avoidanceVelocity;
}

sf::Vector2f Agent::calculateAttractionVelocity(const std::vector<Agent>& neighbours) {
    sf::Vector2f attractionVelocity{ 0.f, 0.f };

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
        if (distance > safeCompanionDistance)
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
    float maxSpeed = personality.preferredSpeed * maxSpeedMultiplier;
    if (speed > maxSpeed)
    {
        velocity = VectorUtils::normalise(velocity) * maxSpeed;
    }
}

void Agent::move(const Area& area, float deltaTime) {
    // update position after deltaTime seconds
    position += velocity * deltaTime;
    // limit movement to within the bounds of the simulation area
    area.moveWithinBounds(position, agentRadius);
}
