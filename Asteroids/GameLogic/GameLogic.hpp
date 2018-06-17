#pragma once

#include "SFML\System\Vector2.hpp"

#include "Physics\Physics.hpp"

class GameLogic;

class GameLogicObject final
{
public:
	GameLogicObject(GameLogic* logic, const sf::Vector2f& position, bool isPlayer = false);

	constexpr bool isPlayer() const { return isPlayer_; }
private:
	GameLogic* gameLogic_;
	PhysicsBody * physicsBody_;
	sf::Vector2f position_;
	bool isPlayer_;
};

class GameLogic final
{
public:
	GameLogic(const Physics& physics);

	int update();

	Physics& getPhysics() { return physics_; }

private:
	void init();

	int onBodiesCollision(const PhysicsBody& body1, const PhysicsBody& body2);

private:
	std::unique_ptr<GameLogicObject> player_;
	Physics physics_;

};