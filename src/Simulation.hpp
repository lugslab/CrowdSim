
#pragma once

#include "Agent.hpp"
#include "DebugStats.hpp"

#include <SFML/System/Vector2.hpp>

#include <random>
#include <vector>

using Clock = std::chrono::steady_clock;

class Simulation
{
public:
    Simulation(float width, float height);

    void update(float deltaTime);

    const std::vector<Agent>& getAgents() const;
    const std::vector<sf::Vector2f>& getDestinations() const;
    const DebugStats& getStats() const;

    void reset(std::size_t agentCount);
    std::size_t getAgentCount() const;

private:
    float m_width;
    float m_height;
    
    DebugStats m_stats;

    std::vector<Agent> m_agents;
    std::vector<sf::Vector2f> m_destinations;

    std::mt19937 m_rng;

    sf::Vector2f randomPosition();
    sf::Vector2f randomDestination();

    std::vector<sf::Vector2f> placeDestinations(float width, float height, float offset);

    void initialiseAllAgents(std::size_t agentCount);
    Agent initialiseAgent(std::size_t index);
    void assignCompanions();
    std::vector<std::size_t> filterSociableAgentIndices();
    void joinCompanions(std::size_t index_a, std::size_t index_b);

    void updateAgents(float deltaTime);
    void updateAgentDestinations();
    void updateStats(Clock::time_point simulation_start);
};
