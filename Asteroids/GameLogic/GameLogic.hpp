#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>

#include "SFML\System\Vector2.hpp"

#include "Physics\Physics.hpp"


class GameLogic;

enum GameLogicObjectType
{
	PLAYER,
	ASTEROID,
	PROJECTILE,
	BOSS,
};

static std::unordered_map<GameLogicObjectType, LeaveFieldStrategy> LogicTypeToStrategy = 
{
	{PLAYER, KEEP},
	{ASTEROID, DESTROY},
	{PROJECTILE, DESTROY},
	{BOSS, KEEP}
};

class GameLogicObject final
{
public:
	GameLogicObject(GameLogic* logic, const sf::Vector2f& position, GameLogicObjectType type);
	~GameLogicObject();

	constexpr void markForDestruction() { shouldDestroy_ = true; }
	constexpr bool shouldDestroy() const { return shouldDestroy_; }
	constexpr GameLogicObjectType getType() const { return type_; }
private:
	GameLogic* gameLogic_;
	PhysicsBodyPtr physicsBody_;

	bool shouldDestroy_ = false;
	GameLogicObjectType type_;
};
using GameLogicObjectPtr = std::shared_ptr<GameLogicObject> ;




class GameLogic final
{
public:
	explicit GameLogic(Physics* physics);

	void init();
	int update();

	void createGameObject(const sf::Vector2f& position, GameLogicObjectType type);
	Physics* getPhysics() const { return physics_; }

private:
	void onBodiesCollision(const PhysicsBody& body1, const PhysicsBody& body2);

private:
	GameLogicObjectPtr player_;

	std::unordered_set<GameLogicObjectPtr> objects_;
	Physics* physics_;

};