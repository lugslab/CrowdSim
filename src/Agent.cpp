
#include "Agent.hpp"

void Agent::update(float deltaTime, float width, float height)

{
	position += velocity * deltaTime;

	constexpr float radius = 4.f;

	if (position.x < radius)
	{
		position.x = radius;
		velocity.x = -velocity.x;
	}
	else if (position.x > width - radius)
	{
		position.x = width - radius;
		velocity.x = -velocity.x;
	}

	if (position.y < radius)
	{
		position.y = radius;
		velocity.y = -velocity.y;
	}
	else if (position.y > height - radius)
	{
		position.y = height - radius;
		velocity.y = -velocity.y;
	}
}
