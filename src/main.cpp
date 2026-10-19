
#include <SFML/Graphics.hpp>

#include "Simulation.hpp"
#include "DebugPanel.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>

int main()
{
    constexpr unsigned int width = 1000;
    constexpr unsigned int height = 700;

    sf::RenderWindow window(
        sf::VideoMode({ width, height }),
        "CrowdSim - Debug Panel"
    );

    window.setFramerateLimit(60);

    Simulation simulation(
        static_cast<float>(width),
        static_cast<float>(height)
    );

    DebugPanel debugPanel("assets/COUR.ttf");
    
    sf::CircleShape agentShape(4.f);
    agentShape.setOrigin({ 4.f, 4.f });

    sf::CircleShape destinationShape(12.f);
    destinationShape.setOrigin({ 12.f, 12.f });
    destinationShape.setFillColor(
        sf::Color(70, 200, 130, 50)
    );
    destinationShape.setOutlineColor(
        sf::Color(70, 200, 130)
    );
    destinationShape.setOutlineThickness(2.f);

    sf::Clock clock;


    using Clock = std::chrono::steady_clock;

    auto previousFrame = Clock::now();

    while (window.isOpen())
    {
        const auto frameStart = Clock::now();

        float deltaTime =
            std::chrono::duration<float>(
                frameStart - previousFrame
            ).count();

        previousFrame = frameStart;

        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        simulation.update(
            std::min(deltaTime, 0.05f)
        );

        const auto renderStart = Clock::now();

        window.clear(sf::Color(25, 28, 35));

        // Draw destinations.
        for (const auto& destination :
            simulation.getDestinations())
        {
            destinationShape.setPosition(destination);
            window.draw(destinationShape);
        }

        // Draw agents.
        for (const auto& agent :
            simulation.getAgents())
        {
            switch (agent.personality.type)
            {
            case PersonalityType::Cautious:
                agentShape.setFillColor(
                    sf::Color(96, 165, 250));
                break;

            case PersonalityType::Impatient:
                agentShape.setFillColor(
                    sf::Color(251, 146, 60));
                break;

            case PersonalityType::Sociable:
                agentShape.setFillColor(
                    sf::Color(74, 222, 128));
                break;

            case PersonalityType::Independent:
                agentShape.setFillColor(
                    sf::Color(192, 132, 252));
                break;
            }

            agentShape.setPosition(agent.position);
            window.draw(agentShape);
        }

        const auto renderEnd = Clock::now();

        DebugStats stats = simulation.getStats();

        stats.frameTimeMs = deltaTime * 1000.f;

        if (deltaTime > 0.f)
            stats.fps = 1.f / deltaTime;

        stats.renderTimeMs =
            std::chrono::duration<float, std::milli>(
                renderEnd - renderStart
            ).count();

        debugPanel.draw(window, stats);

        window.display();
    }

    return 0;
}
