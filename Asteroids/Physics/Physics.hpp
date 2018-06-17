#pragma once

#include <memory>
#include <vector>

class GameLogicObject;



class PhysicsBody final
{
public:
	constexpr GameLogicObject* getLogicObject() const { return logicObject_; }
private:
	GameLogicObject* logicObject_;
};

using PhysicsBodyPtr = std::shared_ptr<PhysicsBody>;



class Physics final
{
public:
	PhysicsBody* createPhysicsBody();

	int update();

private:
	std::vector<PhysicsBodyPtr> bodies_;
};
