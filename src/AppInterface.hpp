
#include <chrono>

#include <SFML/Graphics.hpp>

#include "DebugPanel.hpp"
#include "Simulation.hpp"
#include "PerformanceMonitor.hpp"

using Clock = std::chrono::steady_clock;

class AppInterface 
{
public:
    AppInterface();

    void runEventLoop();
private:
    sf::RenderWindow window;

    Simulation simulation;
    DebugPanel debugPanel;

    PerformanceMonitor performanceMonitor;

    bool paused = false;
    bool showDebugPanel = true;
    bool benchmarking = false;

    Clock::time_point benchmarkStart;
    Clock::time_point renderStart;
    Clock::time_point renderEnd;

    void handleKeyPressed(const sf::Event::KeyPressed* key);
    void resetSimulation(std::size_t agent_count);
    DebugStats gatherStats(float deltaTime);
    void writeBenchmark();
    void renderFrame(const DebugStats& stats);
    void drawDestinations();
    sf::CircleShape getDestinationShape() const;
    void drawAgents();

};
