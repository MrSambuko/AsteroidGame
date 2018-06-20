#pragma once

#include "SFML/System/Vector2.hpp"
#include <math.h>


constexpr float PI = 3.14159265359f;

static float angleBetweenVectors(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	const float& dot = v1.x*v2.x + v1.y*v2.y;
	const float& det = v1.x*v2.y - v1.y*v2.x;

	return atan2f(det, dot);
}

static sf::Vector2f normalizeVector(const sf::Vector2f& vector)
{
	const float length = sqrtf(vector.x*vector.x + vector.y*vector.y);
	return {vector.x/length, vector.y/length};
}
