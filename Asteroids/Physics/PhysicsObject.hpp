#pragma once

#include <memory>

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Shape.hpp"
#include <SFML/System/Vector3.hpp>

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
	void setRotation(float anglePerSeconds) { anglePerSeconds_ = anglePerSeconds; }
	void setDirection( const sf::Vector2f& newDirection );
	bool intersects(const PhysicsObject& other) const;

	constexpr const sf::Vector2f& getPosition() const { return position_; }
	constexpr const sf::Vector2f& getVelocity() const { return velocity_; }
	constexpr const sf::Vector2f& getDirection() const { return direction_; }
	std::vector<sf::Vector3f> getNormals() const;
	std::shared_ptr<sf::Shape> getShape() const { return shape_; }
protected:
	Physics* physics_;
	GameLogicObject* logicObject_;

	PY::LeaveFieldStrategy strategy_;

	float anglePerSeconds_ = 0.0f;
	sf::Vector2f direction_ = {0.0f, 1.0f};
	sf::Vector2f position_;
	sf::Vector2f velocity_;

	std::shared_ptr<sf::Shape> shape_;
};
using PhysicsObjectPtr = std::shared_ptr<PhysicsObject>;




