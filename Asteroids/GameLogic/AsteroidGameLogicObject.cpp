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

SmallAsteroidGameLogicObject::SmallAsteroidGameLogicObject( GameLogic* logic, const sf::Vector2f& position, const sf::Vector2f& direction, float speed ) :
	GameLogicObject(logic, position, GL::SMALL_ASTEROID)
{
	physicsObject_->setVelocity(direction*speed);
	physicsObject_->setRotation(generateRandomFloat(-.05f, .05f));
}
