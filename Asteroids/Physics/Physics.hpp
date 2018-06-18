#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <unordered_set>

#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\CircleShape.hpp"


class GameLogicObject;
class Physics;


enum LeaveFieldStrategy
{
	DESTROY,
	BOUNCE,
	KEEP
};

class PhysicsBody final
{
public:
	PhysicsBody(Physics* physics, GameLogicObject* logicObject, const sf::Vector2f&& position, LeaveFieldStrategy strategy);
	constexpr GameLogicObject* getLogicObject() const { return logicObject_; }

	void move();
	void reverseVelocity();
	void setVelocity(const sf::Vector2f&& newVelocity) { velocity_ = newVelocity; }
	bool intersects(const PhysicsBody& other) const;
private:
	Physics* physics_;
	GameLogicObject* logicObject_;

	LeaveFieldStrategy strategy_;

	sf::Vector2f position_;
	sf::Vector2f velocity_;

	sf::CircleShape shape_;
};

using PhysicsBodyPtr = std::shared_ptr<PhysicsBody>;


using CollisionCallback = std::function<void(const PhysicsBody&, const PhysicsBody&)>;

class Physics final
{
public:
	Physics(float width, float height) : width_(width), height_(height) {}

	sf::Vector2f getBounds() const { return {width_, height_}; }
	
	PhysicsBodyPtr createPhysicsBody(GameLogicObject* obj, const sf::Vector2f& position, LeaveFieldStrategy strategy);
	void destroyPhysicsBody(const PhysicsBodyPtr body);

	void setCollistionCallback(const CollisionCallback& callback)
	{
		callback_ = callback;
	}

	int update();

private:
	void updateCollisions();
	void updatePositions();

private:
	float width_, height_;
	CollisionCallback callback_;
	std::unordered_set<PhysicsBodyPtr> bodies_;
	std::vector<PhysicsBodyPtr> bodiesToDestroy_;
};
