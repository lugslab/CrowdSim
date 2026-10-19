
#pragma once

#include "Agent.hpp"
#include "DebugStats.hpp"

#include <SFML/System/Vector2.hpp>

#include <random>
#include <vector>

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

    void initialiseAgents(std::size_t agentCount);
    void assignCompanions();
};
