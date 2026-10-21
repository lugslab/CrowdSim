#include <SFML/Graphics.hpp>

#include "DebugPanel.hpp"
#include "Simulation.hpp"
#include "PerformanceMonitor.hpp"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iostream>

class AppInterface 
{
public:
    AppInterface();

    void runEventLoop();
private:
    static constexpr sf::Vector2u windowSize{ 1000, 700 };
    static constexpr float benchmarkDuration = 10.f;

    sf::RenderWindow window;

    Simulation simulation;
    DebugPanel debugPanel;

    PerformanceMonitor performanceMonitor;

    bool paused = false;
    bool showDebugPanel = true;
    bool benchmarking = false;

    using Clock = std::chrono::steady_clock;
    Clock::time_point benchmarkStart;
    Clock::time_point renderStart;
    Clock::time_point renderEnd;

    void handleKeyPressed(const sf::Event::KeyPressed* key);
    DebugStats gatherStats(float deltaTime, const std::chrono::steady_clock::time_point& renderStart);
    void writeBenchmark();
    void renderFrame(float deltaTime, const DebugStats& stats);
    void drawDestinations();
    sf::CircleShape getDestinationShape() const;
    void drawAgents();

};
