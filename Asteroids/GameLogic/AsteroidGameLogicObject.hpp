#pragma once


#include "GameLogicObject.hpp"

class GameLogic;

class AsteroidGameLogicObject : public GameLogicObject
{
public:
	AsteroidGameLogicObject(GameLogic* logic, float speed);
};

class SmallAsteroidGameLogicObject : public GameLogicObject
{
public:
	SmallAsteroidGameLogicObject(GameLogic* logic, const sf::Vector2f& position, const sf::Vector2f& direction, float speed);
};

