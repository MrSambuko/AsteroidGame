#pragma once
#include "PhysicsObject.hpp"


class BossPhysicsObject : public PhysicsObject
{
public:
	BossPhysicsObject(Physics* physics, GameLogicObject* logicObject, const sf::Vector2f& position);
};

