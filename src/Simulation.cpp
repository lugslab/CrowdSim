
#include "Simulation.hpp"

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

    m_agents.reserve(agentCount);

    for (int i = 0; i < agentCount; ++i)
    {
        Agent agent;

        agent.position = randomPosition();
        agent.destination = randomDestination();

        PersonalityType type =
            static_cast<PersonalityType>(i % 4);

        agent.personality = Personality::create(type);

        m_agents.push_back(agent);
    }

    assignCompanions();
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

void Simulation::assignCompanions()
{
    std::vector<std::size_t> sociableIndices;

    for (std::size_t i = 0; i < m_agents.size(); ++i)
    {
        if (m_agents[i].personality.type ==
            PersonalityType::Sociable)
        {
            sociableIndices.push_back(i);
        }
    }

    // Pair sociable agents.
    for (std::size_t i = 0;
        i + 1 < sociableIndices.size();
        i += 2)
    {
        std::size_t a = sociableIndices[i];
        std::size_t b = sociableIndices[i + 1];

        m_agents[a].companionIndex =
            static_cast<int>(b);

        m_agents[b].companionIndex =
            static_cast<int>(a);

        // Give both agents the same destination.
        m_agents[b].destination =
            m_agents[a].destination;

        // Start companions close together.
        m_agents[b].position =
            m_agents[a].position +
            sf::Vector2f{ 12.f, 12.f };

        // Keep the second agent inside the area.
        m_agents[b].position.x = std::min(
            m_agents[b].position.x, m_width - 4.f
        );

        m_agents[b].position.y = std::min(
            m_agents[b].position.y, m_height - 4.f
        );
    }
}

void Simulation::update(float deltaTime)
{
    const std::vector<Agent> snapshot = m_agents;

    for (std::size_t i = 0; i < m_agents.size(); ++i)
    {
        m_agents[i].update(
            deltaTime,
            snapshot,
            i,
            m_width,
            m_height
        );
    }

    // Update destinations after all agents have moved.
    for (std::size_t i = 0; i < m_agents.size(); ++i)
    {
        Agent& agent = m_agents[i];

        if (!agent.hasReachedDestination())
            continue;

        if (agent.companionIndex >= 0)
        {
            // The lower-indexed companion chooses
            // the next shared destination once both
            // members have arrived.
            std::size_t companionIndex =
                static_cast<std::size_t>(
                    agent.companionIndex
                    );

            if (i > companionIndex)
                continue;

            Agent& companion =
                m_agents[companionIndex];

            if (!companion.hasReachedDestination())
                continue;

            sf::Vector2f next = randomDestination();

            agent.destination = next;
            companion.destination = next;
        }
        else
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
