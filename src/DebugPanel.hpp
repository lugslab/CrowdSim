#pragma once

#include "DebugStats.hpp"

#include <SFML/Graphics.hpp>

#include <iosfwd>
#include <string>

class DebugPanel
{
public:
    explicit DebugPanel(const std::string& fontPath);

    bool isLoaded() const;

    void draw(
        sf::RenderTarget& target,
        const DebugStats& stats
    ) const;

private:
    bool m_loaded = false;
    sf::Font m_font;

    // constants
    static constexpr unsigned int fontSize = 15;
    
    static constexpr sf::Vector2f panelSize{ 310.f, 260.f };
    static constexpr sf::Vector2f panelTopLeft{ 10.f, 10.f };
    static const sf::Color panelColour;

    static constexpr sf::Vector2f textTopLeft{ 20.f, 18.f };
    static const sf::Color textColour;

    // helper functions
    void drawBackground(sf::RenderTarget& target) const;
    void outputStatsToStream(std::ostream& stream, const DebugStats& stats) const;
    void drawStreamAsText(sf::RenderTarget& target, const std::ostringstream& stream) const;
};