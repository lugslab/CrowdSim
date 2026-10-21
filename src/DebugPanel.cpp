#include <iomanip>
#include <iostream>
#include <sstream>

#include "DebugPanel.hpp"

// initialise static constants
const sf::Color DebugPanel::panelColour{ 10, 15, 22, 220 };
const sf::Color DebugPanel::textColour = sf::Color::White;

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
) const
{
    if (!m_loaded) {
        std::cout << "DebugPanel font not loaded, cannot draw debug panel." << std::endl;
        return;
    }
    
    std::ostringstream stream;
    outputStatsToStream(stream, stats);

    drawBackground(target);

    drawStreamAsText(target, stream);
}

void DebugPanel::drawBackground(sf::RenderTarget& target) const
{
    sf::RectangleShape background(panelSize);
    background.setPosition(panelTopLeft);
    background.setFillColor(panelColour);

    target.draw(background);
}

void DebugPanel::outputStatsToStream(std::ostream& stream, const DebugStats& stats) const
{
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
}

void DebugPanel::drawStreamAsText(sf::RenderTarget& target, const std::ostringstream& stream) const
{
    sf::Text text(m_font, stream.str(), fontSize);
    text.setPosition(textTopLeft);
    text.setFillColor(textColour);
    target.draw(text);
}
