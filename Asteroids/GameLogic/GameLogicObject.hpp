#pragma once

#include <chrono>
#include <unordered_map>

#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Keyboard.hpp"

#include "Physics/Physics.hpp"



namespace GL
{
enum GameLogicObjectType
{
	PLAYER,
	ASTEROID,
	PROJECTILE,
	BOSS,
};
using MoveDirection = int;

constexpr static MoveDirection NO_MOVE = 0;
constexpr static MoveDirection UP = 0x0001;
constexpr static MoveDirection RIGHT = 0x0010;
constexpr static MoveDirection DOWN = 0x0100;
constexpr static MoveDirection LEFT = 0x1000;

}



class GameLogic;


class GameLogicObject
{
public:
	GameLogicObject(GameLogic* logic, const sf::Vector2f& position, GL::GameLogicObjectType type);
	~GameLogicObject();

	constexpr void markForDestruction() { shouldDestroy_ = true; }
	constexpr bool shouldDestroy() const { return shouldDestroy_; }
	constexpr GL::GameLogicObjectType getType() const { return type_; }

	PhysicsObjectPtr getPhysicalObject() const { return physicsObject_; }

protected:
	GameLogic* gameLogic_ = nullptr;
	PhysicsObjectPtr physicsObject_  = nullptr;

	bool shouldDestroy_ = false;
	GL::GameLogicObjectType type_ = GL::PLAYER;

private:
	GameLogicObject(const GameLogicObject& ) = default;
	GameLogicObject(const GameLogicObject&&) noexcept {}
	const GameLogicObject& operator = (const GameLogicObject& ) const { return *this; }
	const GameLogicObject& operator = (const GameLogicObject&& ) const { return *this; }

};
using GameLogicObjectPtr = std::shared_ptr<GameLogicObject>;


class PlayerGameLogicObject : public GameLogicObject
{
public:
	PlayerGameLogicObject(GameLogic* logic) : GameLogicObject(logic, {}, GL::PLAYER) {}
	
	constexpr void startMoving(GL::MoveDirection direction)
	{
		moveDirection_ |= direction;
	}

	constexpr void stopMoving(GL::MoveDirection direction)
	{
		moveDirection_ ^= direction;
	}

	constexpr void startShooting()
	{
		isShooting_ = true;
	}

	constexpr void stopShooting()
	{
		isShooting_ = false;
	}

	constexpr void setLastTimeShooting(const std::chrono::time_point<std::chrono::steady_clock>& lastTimeShooting)
	{
		lastTimeShooting_ = lastTimeShooting;
	}

	constexpr bool isMoving() const { return moveDirection_ != GL::NO_MOVE; }
	constexpr bool isShooting() const { return isShooting_; }
	constexpr std::chrono::time_point<std::chrono::steady_clock> lastTimeShooting() const { return lastTimeShooting_; }
	constexpr GL::MoveDirection getMoveDirection() const { return moveDirection_; }

private:
	bool isShooting_ = false;
	std::chrono::time_point<std::chrono::steady_clock> lastTimeShooting_;
	GL::MoveDirection moveDirection_ = GL::NO_MOVE;
};


class AsteroidGameLogicObject : public GameLogicObject
{
public:
	AsteroidGameLogicObject(GameLogic* logic, float speed);
};

class ProjectileGameLogicObject : public GameLogicObject
{
public:
	ProjectileGameLogicObject( GameLogic* logic, const sf::Vector2f& position, const sf::Vector2f& direction, float speed );
};