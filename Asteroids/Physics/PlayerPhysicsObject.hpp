#pragma once

#include "PhysicsObject.hpp"


class Physics;
class GameLogicObject;

class PlayerPhysicsObject : public PhysicsObject
{
public:
	PlayerPhysicsObject(Physics* physics, GameLogicObject* logicObject);
};
