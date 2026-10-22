#include <gtest/gtest.h>

#include "Area.hpp"
#include "Simulation.hpp"

TEST(SimulationTest, ConstructorCreates100Agents)
{
    Simulation simulation{ Area{ {1000.f, 700.f}, 10, 7 } };

    const auto& agents = simulation.getAgents();

    EXPECT_EQ(agents.size(), 100u);
}

TEST(SimulationTest, AgentsStartWithinBounds) 
{
    const float areaWidth = 1000.f;
    const float areaHeight = 700.f;
    Simulation simulation{ Area{ {areaWidth, areaHeight}, 10, 7 } };

    const auto& agents = simulation.getAgents();

    ASSERT_EQ(agents.size(), 100u);

    for (const Agent& agent : agents)
    {
        const auto position = agent.position;

        EXPECT_GE(position.x, 0.f);
        EXPECT_LE(position.x, areaWidth);

        EXPECT_GE(position.y, 0.f);
        EXPECT_LE(position.y, areaHeight);
    }
}

TEST(SimulationTest, ConstructorCreatesCorrectDestinations)
{
    Simulation simulation{ Area{ {1000.f, 700.f}, 10, 7 } };

    const auto& destinations = simulation.getDestinations();

    ASSERT_EQ(destinations.size(), 5u);

    EXPECT_FLOAT_EQ(destinations[0].x, 100.f);
    EXPECT_FLOAT_EQ(destinations[0].y, 100.f);

    EXPECT_FLOAT_EQ(destinations[1].x, 100.f);
    EXPECT_FLOAT_EQ(destinations[1].y, 600.f);

    EXPECT_FLOAT_EQ(destinations[2].x, 900.f);
    EXPECT_FLOAT_EQ(destinations[2].y, 100.f);

    EXPECT_FLOAT_EQ(destinations[3].x, 900.f);
    EXPECT_FLOAT_EQ(destinations[3].y, 600.f);

    EXPECT_FLOAT_EQ(destinations[4].x, 500.f);
    EXPECT_FLOAT_EQ(destinations[4].y, 350.f);
}