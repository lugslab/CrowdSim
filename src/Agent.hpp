
#pragma once

#include <SFML/System/Vector2.hpp>

struct Agent
{
	sf::Vector2f position;
	sf::Vector2f velocity;

	float preferredSpeed = 50.f;
	float sociability = 0.5f;
	float riskAversion = 0.5f;

	void update(float deltaTime, float width, float height);
};
