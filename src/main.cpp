
#include <SFML/Graphics.hpp>

#include "Simulation.hpp"

#include <algorithm>
#include <cmath>

int main()
{
    constexpr unsigned int width = 1000;
    constexpr unsigned int height = 700;

    sf::RenderWindow window(
        sf::VideoMode({ width, height }),
        "CrowdSim - Goal Directed Movement"
    );

    window.setFramerateLimit(60);

    Simulation simulation(
        static_cast<float>(width),
        static_cast<float>(height)
    );

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

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float deltaTime = std::min(
            clock.restart().asSeconds(), 0.05f
        );

        simulation.update(deltaTime);

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
            float speed = std::sqrt(
                agent.velocity.x * agent.velocity.x +
                agent.velocity.y * agent.velocity.y
            );

            if (speed > 65.f)
                agentShape.setFillColor(
                    sf::Color(255, 140, 90)
                );
            else if (speed > 40.f)
                agentShape.setFillColor(
                    sf::Color(100, 210, 150)
                );
            else
                agentShape.setFillColor(
                    sf::Color(80, 170, 255)
                );

            agentShape.setPosition(agent.position);
            window.draw(agentShape);
        }

        window.display();
    }

    return 0;
}
