#pragma once

#include <memory>

#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\Shape.hpp"
#include <unordered_map>

class Physics;
class GameLogicObject;

namespace PY
{

enum LeaveFieldStrategy
{
	DESTROY,
	BOUNCE,
	KEEP
};
}


class PhysicsObject
{
public:
	PhysicsObject(Physics* physics, GameLogicObject* logicObject, const sf::Vector2f& position, PY::LeaveFieldStrategy strategy);
	constexpr GameLogicObject* getLogicObject() const { return logicObject_; }

	void move();
	void reverseVelocity();
	void setVelocity(const sf::Vector2f&& newVelocity) { velocity_ = newVelocity; }
	bool intersects(const PhysicsObject& other) const;

	const sf::Vector2f& getPosition() const { return position_; }
protected:
	Physics * physics_;
	GameLogicObject* logicObject_;

	PY::LeaveFieldStrategy strategy_;

	sf::Vector2f orientation_;
	sf::Vector2f position_;
	sf::Vector2f velocity_;

	std::unique_ptr<sf::Shape> shape_;
};
using PhysicsBodyPtr = std::shared_ptr<PhysicsObject>;


class PlayerPhysicsObject : public PhysicsObject
{
public:
	PlayerPhysicsObject(Physics* physics, GameLogicObject* logicObject);
};

class AsteroidPhysicsObject : public PhysicsObject
{
public:
	AsteroidPhysicsObject(Physics* physics, GameLogicObject* logicObject, const sf::Vector2f& position);
};