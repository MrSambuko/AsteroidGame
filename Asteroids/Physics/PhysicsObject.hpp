#pragma once

#include <memory>

#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\CircleShape.hpp"

class Physics;
class GameLogicObject;


enum LeaveFieldStrategy
{
	DESTROY,
	BOUNCE,
	KEEP
};


class PhysicsObject final
{
public:
	PhysicsObject(Physics* physics, GameLogicObject* logicObject, const sf::Vector2f&& position, LeaveFieldStrategy strategy);
	constexpr GameLogicObject* getLogicObject() const { return logicObject_; }

	void move();
	void reverseVelocity();
	void setVelocity(const sf::Vector2f&& newVelocity) { velocity_ = newVelocity; }
	bool intersects(const PhysicsObject& other) const;
private:
	Physics * physics_;
	GameLogicObject* logicObject_;

	LeaveFieldStrategy strategy_;

	sf::Vector2f position_;
	sf::Vector2f velocity_;

	sf::CircleShape shape_;
};

using PhysicsBodyPtr = std::shared_ptr<PhysicsObject>;