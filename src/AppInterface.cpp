#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "AppInterface.hpp"
#include "Area.hpp"

using Clock = std::chrono::steady_clock;

namespace
{
    // helper functions to calculate elapsed time
    float elapsedMilliseconds(
        const Clock::time_point& start,
        const Clock::time_point& end
    )
    {
        return std::chrono::duration<float, std::milli>(end - start).count();
    }

    float elapsedSeconds(
        const Clock::time_point& start,
        const Clock::time_point& end
    )
    {
        return std::chrono::duration<float>(end - start).count();
    }


}

namespace Cfg
{
    // application constants
    namespace AgentColour
    {
        static const sf::Color Cautious{ 96, 165, 250 };
        static const sf::Color Impatient{ 251, 146, 60 };
        static const sf::Color Sociable{ 74, 222, 128 };
        static const sf::Color Independent{ 192, 132, 252 };
    }

    namespace Destination
    {
        static const sf::Color FillColour{ 70, 200, 130, 50 };
        static const sf::Color OutlineColour{ 70, 200, 130 };
        static constexpr float Radius = 12.f;
        static constexpr float Thickness = 2.f;
    }

    namespace Benchmark
    {
        static constexpr float Duration = 10.f;
        constexpr std::array<int, 5> AgentCounts{
            100, 500, 1000, 2500, 5000
        };
    }

    namespace Window
    {
        static const sf::Color BackgroundColour{ 25, 28, 35 };
        static constexpr float MaxDeltaTime = 0.05f;
    }
}

AppInterface::AppInterface()
// Initialise the SFML window
    : window(
        sf::VideoMode({ Area::getWidth(), Area::getHeight() }),
        "CrowdSim - Debug Panel"
    )

    // Initialise the debug panel with its font path
    , debugPanel("assets/COUR.ttf")

{
    window.setFramerateLimit(60);
    // Check if the debug panel font loaded successfully
    if (!debugPanel.isLoaded()) {
        std::cerr << "DebugPanel font not loaded, cannot draw debug panel."
          << std::endl;
    }
}

void AppInterface::runEventLoop()
{

    auto previousFrame = Clock::now();

    while (window.isOpen())
    {
        const auto frameStart = Clock::now();

        float deltaTime = elapsedSeconds(previousFrame, frameStart);

        previousFrame = frameStart;

        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const sf::Event::KeyPressed* key =
                event->getIf<sf::Event::KeyPressed>())
            {
                handleKeyPressed(key);
            }
        }
        if (!window.isOpen())
        {
            // close event was received, exit the main loop
            break;
        }
        if (!paused)
        {
            //TODO handle pause during benchmarking, currently it will continue to run the benchmark in the background
            simulation.update(std::min(deltaTime, Cfg::Window::MaxDeltaTime));
        }

        // gathering stats before rendering means that render stats are always one frame behind
        DebugStats stats = gatherStats(deltaTime);

        renderFrame(stats);
    }
}

void AppInterface::handleKeyPressed(const sf::Event::KeyPressed* key)
{
    switch (key->code)
    {
    case sf::Keyboard::Key::Num1:
        resetSimulation(Cfg::Benchmark::AgentCounts[0]);
        break;

    case sf::Keyboard::Key::Num2:
        resetSimulation(Cfg::Benchmark::AgentCounts[1]);
        break;

    case sf::Keyboard::Key::Num3:
        resetSimulation(Cfg::Benchmark::AgentCounts[2]);
        break;

    case sf::Keyboard::Key::Num4:
        resetSimulation(Cfg::Benchmark::AgentCounts[3]);
        break;

    case sf::Keyboard::Key::Num5:
        resetSimulation(Cfg::Benchmark::AgentCounts[4]);
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

void AppInterface::resetSimulation(std::size_t agent_count) {
    simulation.reset(agent_count);
    performanceMonitor.reset();
}

DebugStats AppInterface::gatherStats(float deltaTime) {
    DebugStats stats = simulation.getStats();

    stats.frameTimeMs = deltaTime * 1000.f;

    if (deltaTime > 0.f)
        stats.fps = 1.f / deltaTime;

    stats.renderTimeMs = elapsedMilliseconds(renderStart, renderEnd);


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

    if (benchmarking)
    {
        const float elapsed = elapsedSeconds(benchmarkStart, Clock::now());

        if (elapsed >= Cfg::Benchmark::Duration) {
            // stop benchmarking and write results
            benchmarking = false;
            writeBenchmark();
        }
    }
    return stats;
}

    void AppInterface::writeBenchmark()
    {

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


    void AppInterface::renderFrame(const DebugStats& stats) {
        renderStart = Clock::now();

        window.clear(Cfg::Window::BackgroundColour);

        drawDestinations();

        drawAgents();

        if (showDebugPanel)
        {
            debugPanel.draw(window, stats);
        }

        window.display();

        renderEnd = Clock::now();
    }

    void AppInterface::drawDestinations() {
        sf::CircleShape destinationShape = getDestinationShape();
        // Draw destinations.
        for (const auto& destination :
            simulation.getDestinations())
        {
            destinationShape.setPosition(destination);
            window.draw(destinationShape);
        }

    }

    sf::CircleShape AppInterface::getDestinationShape() const
    {
        sf::CircleShape destinationShape(Cfg::Destination::Radius);

        destinationShape.setOrigin({ 
            Cfg::Destination::Radius,
            Cfg::Destination::Radius
        });
        destinationShape.setFillColor(Cfg::Destination::FillColour);
        destinationShape.setOutlineColor(Cfg::Destination::OutlineColour);
        destinationShape.setOutlineThickness(Cfg::Destination::Thickness);

        return destinationShape;
    }

    void AppInterface::drawAgents() {
        // reusable shape for drawing agents
        sf::CircleShape agentShape(Agent::agentRadius);
        // centre origin
        agentShape.setOrigin({ Agent::agentRadius, Agent::agentRadius });

        // Draw agents.
        for (const auto& agent :
            simulation.getAgents())
        {
            // default to independent colour, override for other personality types
            auto colour = Cfg::AgentColour::Independent;
            switch (agent.personality.type)
            {
            case PersonalityType::Cautious:
                colour = Cfg::AgentColour::Cautious; break;
            case PersonalityType::Impatient:
                colour = Cfg::AgentColour::Impatient; break;
            case PersonalityType::Sociable:
                colour = Cfg::AgentColour::Sociable; break;
            }
            agentShape.setFillColor(colour);
            agentShape.setPosition(agent.position);
            window.draw(agentShape);
        }
    }
