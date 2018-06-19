#pragma once

#include "SFML\System\Vector2.hpp"

#include "Physics\Physics.hpp"


class GameLogic;

namespace GL
{
enum GameLogicObjectType
{
	PLAYER,
	ASTEROID,
	PROJECTILE,
	BOSS,
};
};

class GameLogicObject
{
public:
	GameLogicObject(GameLogic* logic, const sf::Vector2f& position, GL::GameLogicObjectType type);
	~GameLogicObject();

	constexpr void markForDestruction() { shouldDestroy_ = true; }
	constexpr bool shouldDestroy() const { return shouldDestroy_; }
	constexpr GL::GameLogicObjectType getType() const { return type_; }
protected:
	GameLogic * gameLogic_;
	PhysicsBodyPtr physicsBody_;

	bool shouldDestroy_ = false;
	GL::GameLogicObjectType type_;
};
using GameLogicObjectPtr = std::shared_ptr<GameLogicObject>;


class PlayerGameLogicObject : public GameLogicObject
{
public:
	PlayerGameLogicObject(GameLogic* logic) : GameLogicObject(logic, {}, GL::PLAYER) {}
};

class AsteroidGameLogicObject : public GameLogicObject
{
public:
	AsteroidGameLogicObject(GameLogic* logic);
};