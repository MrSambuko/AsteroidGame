#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "SFML\System\Vector2.hpp"
#include "SFML\Graphics\CircleShape.hpp"

class GameLogicObject;



class PhysicsBody final
{
public:
	PhysicsBody(GameLogicObject* logicObject, const sf::Vector2f&& position);
	constexpr GameLogicObject* getLogicObject() const { return logicObject_; }

	bool intersects(const PhysicsBody& other) const;
private:
	GameLogicObject* logicObject_;
	sf::Vector2f position_;
	sf::CircleShape shape_;
};

using PhysicsBodyPtr = std::shared_ptr<PhysicsBody>;


using CollisionCallback = std::function<void(const PhysicsBody&, const PhysicsBody&)>;

class Physics final
{
public:
	PhysicsBody* createPhysicsBody(GameLogicObject* obj, const sf::Vector2f& position);

	int update();

private:
	CollisionCallback callback_;
	std::vector<PhysicsBodyPtr> bodies_;
};
