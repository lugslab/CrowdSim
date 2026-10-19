
#pragma once

#include "DebugStats.hpp"

#include <SFML/Graphics.hpp>

#include <string>

class DebugPanel
{
public:
    explicit DebugPanel(const std::string& fontPath);

    bool isLoaded() const;

    void draw(
        sf::RenderTarget& target,
        const DebugStats& stats
    );

private:
    sf::Font m_font;
    bool m_loaded = false;
};
