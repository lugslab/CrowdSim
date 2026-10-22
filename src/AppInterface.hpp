
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
    const Area m_area{ {1000.f, 700.f}, 10, 7 };
    Simulation m_simulation{ m_area };

    sf::RenderWindow m_window;
    DebugPanel m_debugPanel;

    PerformanceMonitor m_performanceMonitor;

    bool m_paused = false;
    bool m_showDebugPanel = true;
    bool m_benchmarking = false;

    Clock::time_point m_benchmarkStart;
    Clock::time_point m_renderStart;
    Clock::time_point m_renderEnd;

    void handleKeyPressed(const sf::Event::KeyPressed* key);
    void resetSimulation(std::size_t agent_count);
    DebugStats gatherStats(float deltaTime);
    void writeBenchmark();
    void renderFrame(const DebugStats& stats);
    void drawDestinations();
    sf::CircleShape getDestinationShape() const;
    void drawAgents();

};
