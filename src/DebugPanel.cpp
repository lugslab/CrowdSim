
#include "DebugPanel.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

DebugPanel::DebugPanel(const std::string& fontPath)
{
    m_loaded = m_font.openFromFile(fontPath);
}

bool DebugPanel::isLoaded() const
{
    return m_loaded;
}

void DebugPanel::draw(
    sf::RenderTarget& target,
    const DebugStats& stats
)
{
    if (!m_loaded)
        std::cout << "DebugPanel font not loaded, cannot draw debug panel." << std::endl;
        return;

    sf::RectangleShape background({310.f, 260.f});
    background.setPosition({10.f, 10.f});
    background.setFillColor(
        sf::Color(10, 15, 22, 220)
    );

    target.draw(background);

    std::ostringstream stream;
    stream << std::fixed << std::setprecision(2);

    stream << "CrowdSim Debug\n\n";

    stream << "FPS: " << stats.fps << '\n';
    stream << "Frame: " << stats.frameTimeMs
           << " ms\n";
    stream << "Simulation: " << stats.simulationTimeMs
           << " ms\n";
    stream << "Rendering: " << stats.renderTimeMs
           << " ms\n\n";

    stream << "Agents: " << stats.agentCount << '\n';
    stream << "Average speed: " << stats.averageSpeed
           << " px/s\n";

    stream << "Neighbour checks: "
           << stats.neighbourChecks << '\n';

    stream << "Interactions: "
           << stats.neighbourInteractions << '\n';

    stream << "\nPersonalities: "
           << stats.personalityCounts[0] << " / "
           << stats.personalityCounts[1] << " / "
           << stats.personalityCounts[2] << " / "
           << stats.personalityCounts[3];

    sf::Text text(m_font, stream.str(), 15);
    text.setPosition({20.f, 18.f});
    text.setFillColor(sf::Color::White);

    target.draw(text);
}
