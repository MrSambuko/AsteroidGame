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
