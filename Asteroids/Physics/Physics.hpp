#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <unordered_set>

#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\CircleShape.hpp"

#include "PhysicsObject.hpp"


using CollisionCallback = std::function<void(const PhysicsObject&, const PhysicsObject&)>;

class Physics final
{
public:
	Physics(float width, float height) : width_(width), height_(height) {}

	sf::Vector2f getBounds() const { return {width_, height_}; }
	static sf::Vector2f generateRandomVelocity(const sf::Vector2f& position, float speed);
	sf::Vector2f generateRandomPositionOutsideBounds() const;
	
	PhysicsObjectPtr createPhysicsBody(GameLogicObject* obj, const sf::Vector2f& position);
	void destroyPhysicsBody(const PhysicsObjectPtr body);

	void setCollistionCallback(const CollisionCallback& callback)
	{
		callback_ = callback;
	}

	int update(float dt);

private:
	void updateCollisions();
	void updatePositions(float dt);

private:
	float width_, height_;
	CollisionCallback callback_;
	std::unordered_set<PhysicsObjectPtr> bodies_;
	std::vector<PhysicsObjectPtr> bodiesToDestroy_;
};
