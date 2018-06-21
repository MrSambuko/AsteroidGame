#pragma once

#include "SFML/System/Vector2.hpp"

#include "PhysicsObject.hpp"


class Physics;
class GameLogicObject;

class ProjectilePhysicsObject : public PhysicsObject
{
public:
	ProjectilePhysicsObject(Physics* physics, GameLogicObject* logicObject, const sf::Vector2f& position);
};
