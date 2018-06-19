#include "GameLogic.hpp"
#include "GameLogicObject.hpp"


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



AsteroidGameLogicObject::AsteroidGameLogicObject( GameLogic* logic) :
	GameLogicObject(logic, logic->getPhysics()->generateRandomPositionOutsideBounds(), GL::ASTEROID)
{
	physicsObject_->setVelocity(gameLogic_->getPhysics()->generateRandomVelocity(physicsObject_->getPosition(), GL::ASTEROID_SPEED));
}
