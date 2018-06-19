#pragma once

#include "SFML\System\Vector2.hpp"

#include "Physics\Physics.hpp"
#include <SFML/Window/Keyboard.hpp>


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

constexpr static int NO_MOVE	= 0;
constexpr static int UP			= 0x0001;
constexpr static int RIGHT		= 0x0010;
constexpr static int DOWN		= 0x0100;
constexpr static int LEFT		= 0x1000;

using MoveDirection = int;

static std::unordered_map<sf::Keyboard::Key, int> keyToDirectionMap =
{
	{sf::Keyboard::W, UP},
	{sf::Keyboard::D, RIGHT},
	{sf::Keyboard::S, DOWN},
	{sf::Keyboard::A, LEFT},
};

constexpr static float PLAYER_SPEED = 1000.f;
constexpr static float ASTEROID_SPEED = 250.f;

static std::unordered_map<int, sf::Vector2f> moveToVectorMap = 
{
	{NO_MOVE, {}},
	{UP,	sf::Vector2f(  .0f, -1.0f)*PLAYER_SPEED},
	{RIGHT, sf::Vector2f( 1.0f,   .0f)*PLAYER_SPEED},
	{DOWN,	sf::Vector2f(  .0f,  1.0f)*PLAYER_SPEED},
	{LEFT,	sf::Vector2f(-1.0f,   .0f)*PLAYER_SPEED}
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

	PhysicsObjectPtr getPhysicalObject() const { return physicsObject_; }
protected:
	GameLogic * gameLogic_;
	PhysicsObjectPtr physicsObject_;

	bool shouldDestroy_ = false;
	GL::GameLogicObjectType type_;
};
using GameLogicObjectPtr = std::shared_ptr<GameLogicObject>;


class PlayerGameLogicObject : public GameLogicObject
{
public:
	PlayerGameLogicObject(GameLogic* logic) : GameLogicObject(logic, {}, GL::PLAYER) {}
	
	void startMoving(GL::MoveDirection direction)
	{
		moveDirection_ |= direction;
	}

	void stopMoving(GL::MoveDirection direction)
	{
		moveDirection_ ^= direction;
	}

	constexpr bool isMoving() const { return moveDirection_ != GL::NO_MOVE; }
	constexpr GL::MoveDirection getMoveDirection() const { return moveDirection_; }

private:
	GL::MoveDirection moveDirection_ = GL::NO_MOVE;
};

class AsteroidGameLogicObject : public GameLogicObject
{
public:
	AsteroidGameLogicObject(GameLogic* logic);
};