#pragma once

#include "SFML/System/Vector2.hpp"
#include <cmath>
#include <SFML/System/Vector3.hpp>


constexpr float PI = 3.14159265359f;

static float angleBetweenVectors(const sf::Vector2f& v1, const sf::Vector2f& v2)
{
	const float& dot = v1.x*v2.x + v1.y*v2.y;
	const float& det = v1.x*v2.y - v1.y*v2.x;

	return atan2f(det, dot);
}

sf::Vector3f crossProduct(const sf::Vector3f& v1, const sf::Vector3f& v2)
{
	return {v1.y*v2.z - v1.z*v2.y,
		    v1.x*v2.z - v1.z*v2.x,
			v1.x*v2.y - v1.y*v1.x};
}

static sf::Vector2f normalizeVector(const sf::Vector2f& vector)
{
	const float length = sqrtf(vector.x*vector.x + vector.y*vector.y);
	return {vector.x/length, vector.y/length};
}

void normalizeVector(sf::Vector3f* vector)
{
	*vector /= sqrtf(vector->x*vector->x + vector->y*vector->y + vector->z*vector->z);
}

static sf::Vector2f getNormal(const sf::Vector2f& point1, const sf::Vector2f& point2)
{
	return {point1.y - point2.y, point2.x - point1.x};
}