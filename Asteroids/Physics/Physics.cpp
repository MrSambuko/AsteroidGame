#include "Physics.hpp"

PhysicsBody* Physics::createPhysicsBody()
{
	auto ptr = std::make_shared<PhysicsBody>();
	bodies_.push_back(ptr);
	return ptr.get();
}

int Physics::update()
{
	return 0;
}
