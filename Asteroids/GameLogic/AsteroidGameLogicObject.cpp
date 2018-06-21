#include "Physics/Physics.hpp"
#include "System/Common.hpp"

#include "GameLogic.hpp"

#include "AsteroidGameLogicObject.hpp"


AsteroidGameLogicObject::AsteroidGameLogicObject( GameLogic* logic, float asteroidSpeed ) :
	GameLogicObject(logic, logic->getPhysics()->generateRandomPositionOutsideBounds(), GL::ASTEROID)
{
	physicsObject_->setVelocity(Physics::generateRandomVelocity(physicsObject_->getPosition(), asteroidSpeed));
	physicsObject_->setRotation(generateRandomFloat(.0f, .01f));
}
