#pragma once


#include "GameLogicObject.hpp"

class GameLogic;

class AsteroidGameLogicObject : public GameLogicObject
{
public:
	AsteroidGameLogicObject(GameLogic* logic, float speed);
};
