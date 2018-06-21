#pragma once

#include "SFML/System/Vector2.hpp""

#include "PhysicsObject.hpp"


class AsteroidPhysicsObject : public PhysicsObject
{
public:
	AsteroidPhysicsObject(Physics* physics, GameLogicObject* logicObject, const sf::Vector2f& position);
};
