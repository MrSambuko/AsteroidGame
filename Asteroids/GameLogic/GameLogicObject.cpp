#include "GameLogic.hpp"
#include "GameLogicObject.hpp"


GameLogicObject::GameLogicObject(GameLogic* logic, const sf::Vector2f& position, GameLogicObjectType type) :
	gameLogic_(logic),
	type_(type)
{
	physicsBody_ = gameLogic_->getPhysics()->createPhysicsBody(this, position, LogicTypeToStrategy[type]);
}

GameLogicObject::~GameLogicObject()
{
	gameLogic_->getPhysics()->destroyPhysicsBody(physicsBody_);
}
