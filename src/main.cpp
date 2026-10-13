
#include <SFML/Graphics.hpp>

#include <vector>

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({1000u, 700u}),
        "CrowdSim"
    );

    window.setFramerateLimit(60);

    // Temporary test agents.
    std::vector<sf::CircleShape> agents;

    for (int i = 0; i < 100; ++i)
    {
        sf::CircleShape dot(4.f);
        dot.setFillColor(sf::Color(80, 190, 255));
        dot.setPosition({
            30.f + static_cast<float>((i % 10) * 90),
            30.f + static_cast<float>((i / 10) * 60)
        });

        agents.push_back(dot);
    }

    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear(sf::Color(25, 28, 35));

        for (const auto& agent : agents)
        {
            window.draw(agent);
        }

        window.display();
    }

    return 0;
}
