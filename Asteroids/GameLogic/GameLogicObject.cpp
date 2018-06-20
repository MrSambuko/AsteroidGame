#include "GameLogic.hpp"

#include "GameLogicObject.hpp"


constexpr static float SHOOT_INTERVAL = 1.f;

GameLogicObject::GameLogicObject(GameLogic* logic, const sf::Vector2f& position, GL::GameLogicObjectType type) :
	gameLogic_(logic),
	type_(type)
{
	physicsObject_ = gameLogic_->getPhysics()->createPhysicsBody(this, position);
}

GameLogicObject::~GameLogicObject()
{
	gameLogic_->getPhysics()->destroyPhysicsBody(physicsObject_);
}



AsteroidGameLogicObject::AsteroidGameLogicObject( GameLogic* logic, float asteroidSpeed ) :
	GameLogicObject(logic, logic->getPhysics()->generateRandomPositionOutsideBounds(), GL::ASTEROID)
{
	physicsObject_->setVelocity(Physics::generateRandomVelocity(physicsObject_->getPosition(), asteroidSpeed));
}

ProjectileGameLogicObject::ProjectileGameLogicObject( GameLogic* logic, const sf::Vector2f& position, const sf::Vector2f& direction, float speed): 
	GameLogicObject(logic, position, GL::PROJECTILE)
{
	physicsObject_->setVelocity(direction * speed);
}
