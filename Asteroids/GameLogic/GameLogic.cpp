
#include "Physics\Physics.hpp"

#include "GameLogic.hpp"



GameLogicObject::GameLogicObject(GameLogic* logic, const sf::Vector2f& position, bool isPlayer) :
	gameLogic_(logic),
	isPlayer_(isPlayer)
{
	physicsBody_ = gameLogic_->getPhysics()->createPhysicsBody(this, position);
}


GameLogic::GameLogic(Physics* physics) :
	physics_(physics),
	player_(nullptr)
{
}

int GameLogic::update()
{
	return 0;
}

void GameLogic::createGameObject(const sf::Vector2f & position)
{
	objects_.emplace_back(GameLogicObjectPtr(new GameLogicObject(this, position)));
}

void GameLogic::init()
{
	player_ = std::unique_ptr<GameLogicObject>(new GameLogicObject(this, { .0f, .0f }, true));
}

void GameLogic::onBodiesCollision(const PhysicsBody& body1, const PhysicsBody& body2)
{
	// if any of the objects is player - game is over
	if (body1.getLogicObject()->isPlayer() || body2.getLogicObject()->isPlayer())
		return;


	return;
}
