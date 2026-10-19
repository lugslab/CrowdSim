
#include <SFML/Graphics.hpp>

#include "DebugPanel.hpp"
#include "Simulation.hpp"
#include "PerformanceMonitor.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>

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

    PerformanceMonitor performanceMonitor;

    bool paused = false;
    bool showDebugPanel = true;
    bool benchmarking = false;

    using Clock = std::chrono::steady_clock;

    Clock::time_point benchmarkStart;
    constexpr float benchmarkDuration = 10.f;
    
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
            
            if (const auto* key =
                event->getIf<sf::Event::KeyPressed>())
            {
                switch (key->code)
                {
                case sf::Keyboard::Key::Num1:
                    simulation.reset(100);
                    performanceMonitor.reset();
                    break;

                case sf::Keyboard::Key::Num2:
                    simulation.reset(500);
                    performanceMonitor.reset();
                    break;

                case sf::Keyboard::Key::Num3:
                    simulation.reset(1000);
                    performanceMonitor.reset();
                    break;

                case sf::Keyboard::Key::Num4:
                    simulation.reset(2500);
                    performanceMonitor.reset();
                    break;

                case sf::Keyboard::Key::Num5:
                    simulation.reset(5000);
                    performanceMonitor.reset();
                    break;

                case sf::Keyboard::Key::Space:
                    paused = !paused;
                    break;

                case sf::Keyboard::Key::F1:
                    showDebugPanel = !showDebugPanel;
                    break;

                case sf::Keyboard::Key::B:
                    benchmarking = true;
                    benchmarkStart = Clock::now();
                    performanceMonitor.reset();
                    std::cout << "Benchmark started\n";
                    break;

                case sf::Keyboard::Key::Escape:
                    window.close();
                    break;

                default:
                    break;
                }
            }
        }
        if (!paused)
        {
            simulation.update(std::min(deltaTime, 0.05f));
        }

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


        performanceMonitor.recordFrame(
            stats.frameTimeMs,
            stats.simulationTimeMs,
            stats.renderTimeMs
        );

        const PerformanceSummary summary =
            performanceMonitor.getSummary();

        stats.fps = summary.averageFps;
        stats.frameTimeMs = summary.averageFrameMs;
        stats.simulationTimeMs = summary.averageSimulationMs;
        stats.renderTimeMs = summary.averageRenderMs;

        if (showDebugPanel)
        {
            debugPanel.draw(window, stats);
        }

        if (benchmarking)
        {
            const float elapsed =
                std::chrono::duration<float>(
                    Clock::now() - benchmarkStart
                ).count();

            if (elapsed >= benchmarkDuration)
            {
                benchmarking = false;

                const auto result =
                    performanceMonitor.getSummary();

                std::cout << "\n=== CrowdSim Benchmark ===\n";
                std::cout << "Agents: "
                    << simulation.getAgentCount() << '\n';

                std::cout << "Frames: "
                    << result.frames << '\n';

                std::cout << "Average FPS: "
                    << result.averageFps << '\n';

                std::cout << "Average frame: "
                    << result.averageFrameMs << " ms\n";

                std::cout << "Average simulation: "
                    << result.averageSimulationMs << " ms\n";

                std::cout << "Average rendering: "
                    << result.averageRenderMs << " ms\n";

                std::cout << "Worst frame: "
                    << result.worstFrameMs << " ms\n";
            }
        }


        window.display();

    }

    return 0;
}
