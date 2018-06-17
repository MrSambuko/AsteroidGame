#pragma once

#include <memory>

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
	bool isPlayer_;
};
using GameLogicObjectPtr = std::shared_ptr<GameLogicObject> ;




class GameLogic final
{
public:
	GameLogic(Physics* physics);

	void init();
	int update();

	void createGameObject(const sf::Vector2f& position);
	Physics* getPhysics() const { return physics_; }

private:
	void onBodiesCollision(const PhysicsBody& body1, const PhysicsBody& body2);

private:
	std::unique_ptr<GameLogicObject> player_;

	std::vector<GameLogicObjectPtr> objects_;
	Physics* physics_;

};