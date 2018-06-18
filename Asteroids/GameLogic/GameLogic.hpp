#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>

#include "SFML\System\Vector2.hpp"

#include "GameLogicObject.hpp"
#include "Physics\Physics.hpp"


static std::unordered_map<GameLogicObjectType, LeaveFieldStrategy> LogicTypeToStrategy = 
{
	{PLAYER, KEEP},
	{ASTEROID, DESTROY},
	{PROJECTILE, DESTROY},
	{BOSS, KEEP}
};



class GameLogic final
{
public:
	explicit GameLogic(Physics* physics);

	void init();
	int update();

	void createGameObject(const sf::Vector2f& position, GameLogicObjectType type);
	Physics* getPhysics() const { return physics_; }

private:
	void destroyObjects();

	void onBodiesCollision(const PhysicsObject& body1, const PhysicsObject& body2);

private:
	GameLogicObjectPtr player_;

	std::unordered_set<GameLogicObjectPtr> objects_;
	Physics* physics_;

};