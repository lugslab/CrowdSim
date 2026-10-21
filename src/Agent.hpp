#pragma once

#include "Personality.hpp"

#include <gtest/gtest_prod.h>
#include <SFML/System/Vector2.hpp>

#include <cstddef>
#include <vector>

class Agent
{
public:

    sf::Vector2f position{};
    sf::Vector2f velocity{};
    sf::Vector2f destination{};
    // configuration
    static constexpr float agentRadius = 4.f;

    Personality personality{};

    // Index of a companion in the simulation.
    // -1 means no companion.
    int companionIndex = -1;

    void update(
        float deltaTime,
        const std::vector<Agent>& neighbours,
        std::size_t& neighbourChecks,
        std::size_t& neighbourInteractions
    );

    bool hasReachedDestination() const;
private:
    float maxAcceleration = 150.f;
    static constexpr float destinationTolerance = 12.f;
    static constexpr float goalApproachRadius = 50.f;
    static constexpr float personalSpaceMultiplier = 3.f;
    static constexpr float safeCompanionDistance = 45.f;
    static constexpr float maxSpeedMultiplier = 1.5f;
   
    sf::Vector2f calculateGoalVelocity() const;
    sf::Vector2f calculateAvoidanceVelocity(const std::vector<Agent>& neighbours, std::size_t& neighbourChecks, std::size_t& neighbourInteractions);
    sf::Vector2f calculateAttractionVelocity(const std::vector<Agent>& neighbours);
    sf::Vector2f resolveTargetVelocity(sf::Vector2f goalVelocity, sf::Vector2f proximityAvoidance, sf::Vector2f socialAttraction);
    void applyAcceleration(sf::Vector2f targetVelocity, float deltaTime);
    void clampSpeed();
    void move(float deltaTime);

    // provide access for tests
    // TODO re-evaluate code structure and testing approach
    FRIEND_TEST(AgentTests, GoalVelocityPointsTowardDestination);
    FRIEND_TEST(AgentTests, GoalVelocitySlowsDownNearDestination);
    FRIEND_TEST(AgentTests, AvoidanceVelocityIgnoresNeighboursOutsideDetectionRadius);
    FRIEND_TEST(AgentTests, AvoidanceVelocityRepelsNearbyNeighbour);
    FRIEND_TEST(AgentTests, AvoidanceVelocityIgnoresTheAgentItself);
    FRIEND_TEST(AgentTests, SociableAgentIsAttractedToDistantCompanion);
    FRIEND_TEST(AgentTests, AgentIsNotAttractedToCompanionWithinFollowDistance);
    FRIEND_TEST(AgentTests, InvalidCompanionIndexProducesNoAttraction);
    FRIEND_TEST(AgentTests, ResolveTargetVelocityCombinesInfluences);
    FRIEND_TEST(AgentTests, AccelerationIsLimitedToMaximumAcceleration);
    FRIEND_TEST(AgentTests, SpeedIsLimitedToOneAndAHalfTimesPreferredSpeed);
    FRIEND_TEST(AgentTests, MoveKeepsAgentInsideSimulationBounds);
    FRIEND_TEST(AgentTests, MoveClampsAgentToMaximumBounds);
};
