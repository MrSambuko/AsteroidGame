#include "GameLogic.hpp"
#include "GameLogicObject.hpp"


GameLogicObject::GameLogicObject(GameLogic* logic, const sf::Vector2f& position, GL::GameLogicObjectType type) :
	gameLogic_(logic),
	type_(type)
{
	physicsBody_ = gameLogic_->getPhysics()->createPhysicsBody(this, position, LeaveLogicTypeToStrategy[type]);
}

GameLogicObject::~GameLogicObject()
{
	gameLogic_->getPhysics()->destroyPhysicsBody(physicsBody_);
}


AsteroidGameLogicObject::AsteroidGameLogicObject( GameLogic* logic) :
	GameLogicObject(logic, logic->getPhysics()->generateRandomPositionOutsideBounds(), GL::ASTEROID)
{
	physicsBody_->setVelocity(gameLogic_->getPhysics()->generateRandomVelocity(physicsBody_->getPosition(), 5.f));
}
