#pragma once

#include <memory>

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Shape.hpp"

namespace PY
{

enum LeaveFieldStrategy
{
	DESTROY,
	BOUNCE,
	KEEP
};
}

class Physics;
class GameLogicObject;


class PhysicsObject
{
public:
	PhysicsObject(Physics* physics, GameLogicObject* logicObject, const sf::Vector2f& position, PY::LeaveFieldStrategy strategy);
	constexpr GameLogicObject* getLogicObject() const { return logicObject_; }

	void move(const float& dt);
	void reverseVelocity();
	void setVelocity(sf::Vector2f&& newVelocity) { velocity_ = std::move(newVelocity); }
	void setDirection( const sf::Vector2f& newDirection );
	bool intersects(const PhysicsObject& other) const;

	constexpr const sf::Vector2f& getPosition() const { return position_; }
	constexpr const sf::Vector2f& getDirection() const { return direction_; }
	std::shared_ptr<sf::Shape> getShape() const { return shape_; }
protected:
	Physics* physics_;
	GameLogicObject* logicObject_;

	PY::LeaveFieldStrategy strategy_;

	sf::Vector2f direction_ = {0.0f, 1.0f};
	sf::Vector2f position_;
	sf::Vector2f velocity_;

	std::shared_ptr<sf::Shape> shape_;
};
using PhysicsObjectPtr = std::shared_ptr<PhysicsObject>;


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

class ProjectilePhysicsObject : public PhysicsObject
{
public:
	ProjectilePhysicsObject(Physics* physics, GameLogicObject* logicObject, const sf::Vector2f& position);
};