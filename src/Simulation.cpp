
#include "Simulation.hpp"

#include <algorithm>
#include <random>

Simulation::Simulation(float width, float height)
    : m_width(width),
    m_height(height),
    m_rng(std::random_device{}())
{
    m_destinations = {
        {100.f, 100.f},
        {width - 100.f, 100.f},
        {width - 100.f, height - 100.f},
        {100.f, height - 100.f},
        {width / 2.f, height / 2.f}
    };

    constexpr int agentCount = 100;

    std::uniform_real_distribution<float> speedDist(
        25.f, 85.f
    );

    m_agents.reserve(agentCount);

    for (int i = 0; i < agentCount; ++i)
    {
        Agent agent;

        agent.position = randomPosition();
        agent.destination = randomDestination();
        agent.preferredSpeed = speedDist(m_rng);

        m_agents.push_back(agent);
    }
}

sf::Vector2f Simulation::randomPosition()
{
    std::uniform_real_distribution<float> xDist(
        10.f, m_width - 10.f
    );

    std::uniform_real_distribution<float> yDist(
        10.f, m_height - 10.f
    );

    return { xDist(m_rng), yDist(m_rng) };
}

sf::Vector2f Simulation::randomDestination()
{
    std::uniform_int_distribution<std::size_t> dist(
        0, m_destinations.size() - 1
    );

    return m_destinations[dist(m_rng)];
}

void Simulation::update(float deltaTime)
{
    // Snapshot ensures simultaneous decision-making.
    const std::vector<Agent> snapshot = m_agents;

    for (std::size_t i = 0; i < m_agents.size(); ++i)
    {
        Agent& agent = m_agents[i];

        agent.update(
            deltaTime,
            snapshot,
            i,
            m_width,
            m_height
        );

        if (agent.hasReachedDestination())
        {
            agent.destination = randomDestination();
        }
    }
}

const std::vector<Agent>& Simulation::getAgents() const
{
    return m_agents;
}

const std::vector<sf::Vector2f>&
Simulation::getDestinations() const
{
    return m_destinations;
}
