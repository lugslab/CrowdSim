#include <gtest/gtest.h>
#include "Simulation.hpp"

TEST(SimulationTest, ConstructorCreates100Agents)
{
    Simulation simulation(800.f, 600.f);

    const auto& agents = simulation.getAgents();

    EXPECT_EQ(agents.size(), 100u);
}

TEST(SimulationTest, AgentsStartWithinBounds)
{
    constexpr float width = 800.f;
    constexpr float height = 600.f;

    Simulation simulation(width, height);

    const auto& agents = simulation.getAgents();

    ASSERT_EQ(agents.size(), 100u);

    for (const Agent& agent : agents)
    {
        const auto position = agent.position;

        EXPECT_GE(position.x, 0.f);
        EXPECT_LE(position.x, width);

        EXPECT_GE(position.y, 0.f);
        EXPECT_LE(position.y, height);
    }
}

TEST(SimulationTest, ConstructorCreatesCorrectDestinations)
{
    Simulation simulation(800.f, 600.f);

    const auto& destinations = simulation.getDestinations();

    ASSERT_EQ(destinations.size(), 5u);

    EXPECT_FLOAT_EQ(destinations[0].x, 100.f);
    EXPECT_FLOAT_EQ(destinations[0].y, 100.f);

    EXPECT_FLOAT_EQ(destinations[1].x, 700.f);
    EXPECT_FLOAT_EQ(destinations[1].y, 100.f);

    EXPECT_FLOAT_EQ(destinations[2].x, 700.f);
    EXPECT_FLOAT_EQ(destinations[2].y, 500.f);

    EXPECT_FLOAT_EQ(destinations[3].x, 100.f);
    EXPECT_FLOAT_EQ(destinations[3].y, 500.f);

    EXPECT_FLOAT_EQ(destinations[4].x, 400.f);
    EXPECT_FLOAT_EQ(destinations[4].y, 300.f);
}