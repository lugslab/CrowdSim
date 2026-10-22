#include <gtest/gtest.h>

#include "Agent.hpp"

#include <vector>

namespace
{
    void expectVectorNear(
        const sf::Vector2f& actual,
        const sf::Vector2f& expected,
        float tolerance = 0.001f)
    {
        EXPECT_NEAR(actual.x, expected.x, tolerance);
        EXPECT_NEAR(actual.y, expected.y, tolerance);
    }
}

TEST(AgentTests, HasReachedDestinationWhenWithinTwelveUnits)
{
    Agent agent;
    agent.position = { 0.f, 0.f };
    agent.destination = { 11.999f, 0.f };

    EXPECT_TRUE(agent.hasReachedDestination());
}

TEST(AgentTests, HasNotReachedDestinationAtOrBeyondTwelveUnits)
{
    Agent agent;
    agent.position = { 0.f, 0.f };
    agent.destination = { 12.f, 0.f };

    EXPECT_FALSE(agent.hasReachedDestination());
}

TEST(AgentTests, GoalVelocityPointsTowardDestination)
{
    Agent agent;
    agent.position = { 0.f, 0.f };
    agent.destination = { 100.f, 0.f };
    agent.personality = Personality::create(PersonalityType::Independent);

    const sf::Vector2f goalVelocity = agent.calculateGoalVelocity();

    expectVectorNear(goalVelocity, { 60.f, 0.f });
}

TEST(AgentTests, GoalVelocitySlowsDownNearDestination)
{
    Agent agent;
    agent.position = { 0.f, 0.f };
    agent.destination = { 25.f, 0.f };
    agent.personality = Personality::create(PersonalityType::Independent);

    const sf::Vector2f goalVelocity = agent.calculateGoalVelocity();

    // 60 preferred speed * (25 / 50 slowing factor).
    expectVectorNear(goalVelocity, { 30.f, 0.f });
}

TEST(AgentTests, AvoidanceVelocityIgnoresNeighboursOutsideDetectionRadius)
{
    Agent agent;
    agent.position = { 0.f, 0.f };
    agent.personality = Personality::create(PersonalityType::Independent);

    Agent distantNeighbour;
    distantNeighbour.position = { 31.f, 0.f };

    std::size_t neighbourChecks = 0;
    std::size_t neighbourInteractions = 0;

    const sf::Vector2f avoidance = agent.calculateAvoidanceVelocity(
        { distantNeighbour },
        neighbourChecks,
        neighbourInteractions);

    expectVectorNear(avoidance, { 0.f, 0.f });
    EXPECT_EQ(neighbourChecks, 1u);
    EXPECT_EQ(neighbourInteractions, 0u);
}

TEST(AgentTests, AvoidanceVelocityRepelsNearbyNeighbour)
{
    Agent agent;
    agent.position = { 0.f, 0.f };
    agent.personality = Personality::create(PersonalityType::Independent);

    Agent neighbour;
    neighbour.position = { 10.f, 0.f };

    std::size_t neighbourChecks = 0;
    std::size_t neighbourInteractions = 0;

    const sf::Vector2f avoidance = agent.calculateAvoidanceVelocity(
        { neighbour },
        neighbourChecks,
        neighbourInteractions);

    expectVectorNear(avoidance, { -72.f, 0.f });
    EXPECT_EQ(neighbourChecks, 1u);
    EXPECT_EQ(neighbourInteractions, 1u);
}

TEST(AgentTests, AvoidanceVelocityIgnoresTheAgentItself)
{
    Agent agent;
    agent.position = { 10.f, 10.f };
    agent.personality = Personality::create(PersonalityType::Independent);

    std::size_t neighbourChecks = 0;
    std::size_t neighbourInteractions = 0;

    const sf::Vector2f avoidance = agent.calculateAvoidanceVelocity(
        { agent },
        neighbourChecks,
        neighbourInteractions);

    expectVectorNear(avoidance, { 0.f, 0.f });
    EXPECT_EQ(neighbourChecks, 1u);
    EXPECT_EQ(neighbourInteractions, 0u);
}

TEST(AgentTests, SociableAgentIsAttractedToDistantCompanion)
{
    Agent agent;
    agent.position = { 0.f, 0.f };
    agent.personality = Personality::create(PersonalityType::Sociable);
    agent.companionIndex = 0;

    Agent companion;
    companion.position = { 50.f, 0.f };

    const sf::Vector2f attraction =
        agent.calculateAttractionVelocity({ companion });

    expectVectorNear(attraction, { 40.f, 0.f });
}

TEST(AgentTests, AgentIsNotAttractedToCompanionWithinFollowDistance)
{
    Agent agent;
    agent.position = { 0.f, 0.f };
    agent.personality = Personality::create(PersonalityType::Sociable);
    agent.companionIndex = 0;

    Agent companion;
    companion.position = { 45.f, 0.f };

    const sf::Vector2f attraction =
        agent.calculateAttractionVelocity({ companion });

    expectVectorNear(attraction, { 0.f, 0.f });
}

TEST(AgentTests, InvalidCompanionIndexProducesNoAttraction)
{
    Agent agent;
    agent.position = { 0.f, 0.f };
    agent.personality = Personality::create(PersonalityType::Sociable);
    agent.companionIndex = 1;

    Agent companion;
    companion.position = { 100.f, 0.f };

    const sf::Vector2f attraction =
        agent.calculateAttractionVelocity({ companion });

    expectVectorNear(attraction, { 0.f, 0.f });
}

TEST(AgentTests, ResolveTargetVelocityCombinesInfluences)
{
    Agent agent;

    const sf::Vector2f result = agent.resolveTargetVelocity(
        { 10.f, 2.f },
        { -3.f, 4.f },
        { 1.f, -2.f });

    expectVectorNear(result, { 8.f, 4.f });
}

TEST(AgentTests, AccelerationIsLimitedToMaximumAcceleration)
{
    Agent agent;
    agent.velocity = { 0.f, 0.f };
    agent.maxAcceleration = 100.f;

    agent.applyAcceleration({ 300.f, 0.f }, 0.5f);

    expectVectorNear(agent.velocity, { 50.f, 0.f });
}

TEST(AgentTests, SpeedIsLimitedToOneAndAHalfTimesPreferredSpeed)
{
    Agent agent;
    agent.personality = Personality::create(PersonalityType::Independent);
    agent.velocity = { 200.f, 0.f };

    agent.clampSpeed();

    expectVectorNear(agent.velocity, { 90.f, 0.f });
}

TEST(AgentTests, MoveKeepsAgentInsideSimulationBounds)
{
    Area area({ 1000.f, 700.f }, 10, 7);
    Agent agent;
    agent.position = { 10.f, 10.f };
    agent.velocity = { -100.f, -100.f };

    agent.move(area, 1.f);

    expectVectorNear(agent.position, { 4.f, 4.f });
}

TEST(AgentTests, MoveClampsAgentToMaximumBounds)
{
    Area area({ 1000.f, 700.f }, 10, 7);
    Agent agent;
    agent.position = { 990.f, 650.f };
    agent.velocity = { 100.f, 100.f };

    agent.move(area, 1.f);

    expectVectorNear(agent.position, { 996.f, 696.f });
}

TEST(AgentTests, UpdateMovesAgentTowardDestination)
{
    Area area({ 1000.f, 700.f }, 10, 7);
    Agent agent;
    agent.position = { 100.f, 100.f };
    agent.destination = { 200.f, 100.f };
    agent.personality = Personality::create(PersonalityType::Independent);

    std::size_t neighbourChecks = 0;
    std::size_t neighbourInteractions = 0;

    agent.update(
        area,
        0.1f,
        {},
        neighbourChecks,
        neighbourInteractions);

    EXPECT_GT(agent.position.x, 100.f);
    EXPECT_FLOAT_EQ(agent.position.y, 100.f);
    EXPECT_EQ(neighbourChecks, 0u);
    EXPECT_EQ(neighbourInteractions, 0u);
}