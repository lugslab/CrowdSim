
#include <SFML/Graphics.hpp>

#include "Agent.hpp"

#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

int main()
{
	constexpr unsigned int width = 1000;
	constexpr unsigned int height = 700;
	constexpr float radius = 4.f;
	constexpr int agentCount = 100;

	sf::RenderWindow window(
		sf::VideoMode({ width, height }),
		"CrowdSim"
	);

	window.setFramerateLimit(60);

	std::mt19937 rng(std::random_device{}());

	std::uniform_real_distribution<float> xDist(
		radius, static_cast<float>(width) - radius
	);

	std::uniform_real_distribution<float> yDist(
		radius, static_cast<float>(height) - radius
	);

	std::uniform_real_distribution<float> angleDist(
		0.f, 6.2831853f
	);

	std::uniform_real_distribution<float> speedDist(
		25.f, 85.f
	);

	std::vector<Agent> agents;
	agents.reserve(agentCount);

	for (int i = 0; i < agentCount; ++i)
	{
		Agent agent;

		agent.position = { xDist(rng), yDist(rng) };

		float angle = angleDist(rng);
		float speed = speedDist(rng);

		agent.desiredDirection = {
			std::cos(angle),
			std::sin(angle)
		};

		agent.velocity =
			agent.desiredDirection * speed;

		agent.preferredSpeed = speed;

		agents.push_back(agent);
	}

	sf::CircleShape dot(radius);
	dot.setOrigin({ radius, radius });

	sf::Clock clock;

	while (window.isOpen())
	{
		while (const auto event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>())
				window.close();
		}

		float deltaTime = std::min(
			clock.restart().asSeconds(), 0.05f
		);

		// Everyone observes the same world state.
		const std::vector<Agent> snapshot = agents;

		for (auto& agent : agents)
		{
			agent.update(
				deltaTime,
				snapshot,
				static_cast<float>(width),
				static_cast<float>(height)
			);
		}

		window.clear(sf::Color(25, 28, 35));

		for (const auto& agent : agents)
		{
			float speed = std::sqrt(
				agent.velocity.x * agent.velocity.x +
				agent.velocity.y * agent.velocity.y
			);

			if (speed > 65.f)
				dot.setFillColor(
					sf::Color(255, 140, 90)
				);
			else if (speed > 40.f)
				dot.setFillColor(
					sf::Color(100, 210, 150)
				);
			else
				dot.setFillColor(
					sf::Color(80, 170, 255)
				);

			dot.setPosition(agent.position);
			window.draw(dot);
		}

		window.display();
	}

	return 0;
}
