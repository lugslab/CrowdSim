#include <algorithm>
#include <chrono>
#include <numeric>
#include <random>

#include "AgentRange.hpp"
#include "Area.hpp"
#include "Simulation.hpp"


namespace Cfg {
    static constexpr float DestinationOffset = 100.f;
    static constexpr float CompanionOffset = 12.f;
    static constexpr std::size_t AgentCount = 100;
}

Simulation::Simulation(const Area& area)
    : m_area(area), m_rng(std::random_device{}())
{
    m_destinations = m_area.placeDestinations(Cfg::DestinationOffset);

    initialiseAllAgents(Cfg::AgentCount);

}


void Simulation::initialiseAllAgents(std::size_t agentCount)
{
    m_agents.clear();
    m_agents.reserve(agentCount);

    for (std::size_t i = 0; i < agentCount; ++i)
    {
        m_agents.push_back(initialiseAgent(i));

    }
    assignCompanions();
}

Agent Simulation::initialiseAgent(std::size_t index) {
    Agent agent;

    agent.position = randomPosition();
    agent.destination = randomDestination();

    const auto type = static_cast<PersonalityType>(index % 4);

    agent.personality = Personality::create(type);

    return agent;
}

void Simulation::reset(std::size_t agentCount)
{
    initialiseAllAgents(agentCount);

    m_stats = DebugStats{};
    m_stats.agentCount = m_agents.size();
}

std::size_t Simulation::getAgentCount() const
{
    return m_agents.size();
}

sf::Vector2f Simulation::randomPosition()
{
    std::uniform_real_distribution<float> xDist(
        Agent::agentRadius, m_area.getWidth() - Agent::agentRadius
    );

    std::uniform_real_distribution<float> yDist(
        Agent::agentRadius, m_area.getHeight() - Agent::agentRadius
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
    std::vector<std::size_t> sociableIndices = filterSociableAgentIndices();
    for (std::size_t i = 0; i + 1 < sociableIndices.size(); i += 2)
    {
        joinCompanions(
            sociableIndices[i],
            sociableIndices[i + 1]
        );
    }
}

std::vector<std::size_t> Simulation::filterSociableAgentIndices()
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
    return sociableIndices;
}

void Simulation::joinCompanions(std::size_t index_a, std::size_t index_b)
{
    // Pair sociable agents
    auto& agent_a = m_agents[index_a];
    auto& agent_b = m_agents[index_b];

    agent_a.companionIndex =
        static_cast<int>(index_b);

    agent_b.companionIndex =
        static_cast<int>(index_a);

    // Give both agents the same destination.
    agent_b.destination =
        agent_a.destination;

    // Start companions close together.
   agent_b.position =
        agent_a.position +
       sf::Vector2f{ Cfg::CompanionOffset, Cfg::CompanionOffset };

    // Keep the second agent inside the area.
   m_area.moveWithinBounds(
       agent_b.position,
       Agent::agentRadius
   );
}

void Simulation::update(float deltaTime)
{

    const auto simulation_start = Clock::now();

    // Reset per-update counters.
    m_stats.neighbourChecks = 0;
    m_stats.neighbourInteractions = 0;

    updateAgents(deltaTime);
    updateAgentDestinations();
    updateStats(simulation_start);
}


std::vector<std::vector<std::size_t>>
Simulation::populateRegionNeighbourhoods() const
{
    const auto regionSnapshots = populateRegionSnapshots();

    std::vector<std::vector<std::size_t>> neighbourhoodSnapshots(m_area.getRegionCount());

    for (std::size_t i = 0; i < regionSnapshots.size(); ++i)
    {
        const auto neighbourhoodRegionIndices =
            m_area.getRegionAndNeighbourIndices(i);

        std::vector<std::size_t> neighbourhood;

        for (const std::size_t regionIndex : neighbourhoodRegionIndices)
        {
            const auto& agents = regionSnapshots.at(regionIndex);

            neighbourhood.insert(
                neighbourhood.end(),
                agents.begin(),
                agents.end()
            );
        }

        neighbourhoodSnapshots.push_back(std::move(neighbourhood));
    }

    return neighbourhoodSnapshots;
}

std::vector<std::vector<std::size_t>>
Simulation::populateRegionSnapshots() const
{
    std::vector<std::vector<std::size_t>> regionSnapshots(
        m_area.getRegionCount()
    );

    for (std::size_t i = 0; i < m_agents.size(); ++i)
    {
        const std::size_t regionIndex =
            m_area.regionIndexAtPosition(m_agents[i].position);

        regionSnapshots.at(regionIndex).push_back(i);
    }

    return regionSnapshots;
}


void Simulation::updateAgents(float deltaTime)
{
    // capture state of all agents at simulation stat
    const std::vector<Agent> globalSnapshot = m_agents;

    std::vector<std::size_t> allIndices;
    std::vector<std::vector<::size_t>> neighbourhoodIndices;

    if (m_useRegions)
    {
        neighbourhoodIndices = populateRegionNeighbourhoods();
    }
    else
    {
        allIndices.resize(globalSnapshot.size());

        std::iota(
            allIndices.begin(),
            allIndices.end(),
            std::size_t{ 0 }
        );
    }
    
    for (std::size_t i = 0; i < m_agents.size(); ++i)
    {
        // resolve correct snapshot for this agent
        
        // identify region, or skip calculation
        const std::size_t regionIndex = m_useRegions
            ? m_area.regionIndexAtPosition(globalSnapshot[i].position)
            : 0;
        // use region indices, or all indices
        const auto& indices = m_useRegions
            ? neighbourhoodIndices.at(regionIndex)
            : allIndices;

        const AgentRange neighbours(globalSnapshot, indices);


        // update agent
        m_agents[i].update(
            m_area,
            deltaTime,
            globalSnapshot,
            neighbours,
            m_stats.neighbourChecks,
            m_stats.neighbourInteractions
        );
    }

}

void Simulation::updateAgentDestinations()
{

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

void Simulation::updateStats(Clock::time_point start)
{
    m_stats.agentCount = m_agents.size();
    m_stats.personalityCounts.fill(0);

    float totalSpeed = 0.f;

    for (const auto& agent : m_agents)
    {
        totalSpeed += agent.velocity.length();

        const auto index =
            static_cast<std::size_t>(
                agent.personality.type
                );

        ++m_stats.personalityCounts[index];
    }

    if (!m_agents.empty())
    {
        m_stats.averageSpeed =
            totalSpeed / m_agents.size();
    }

    const auto end = Clock::now();

    m_stats.simulationTimeMs =
        std::chrono::duration<float, std::milli>(
            end - start
        ).count();
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

const DebugStats& Simulation::getStats() const
{
    return m_stats;
}