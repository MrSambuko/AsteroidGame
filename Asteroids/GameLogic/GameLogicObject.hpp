#pragma once

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



class GameLogicObject final
{
public:
	GameLogicObject(GameLogic* logic, const sf::Vector2f& position, GameLogicObjectType type);
	~GameLogicObject();

	constexpr void markForDestruction() { shouldDestroy_ = true; }
	constexpr bool shouldDestroy() const { return shouldDestroy_; }
	constexpr GameLogicObjectType getType() const { return type_; }
private:
	GameLogic * gameLogic_;
	PhysicsBodyPtr physicsBody_;

	bool shouldDestroy_ = false;
	GameLogicObjectType type_;
};
using GameLogicObjectPtr = std::shared_ptr<GameLogicObject>;