
#include "Physics\Physics.hpp"

#include "GameLogic.hpp"


GameLogicObject::GameLogicObject(GameLogic* logic, const sf::Vector2f& position, bool isPlayer) :
	gameLogic_(logic),
	position_(position),
	isPlayer_(isPlayer)
{
	physicsBody_ = gameLogic_->getPhysics().createPhysicsBody();
}

GameLogic::GameLogic(const Physics& physics) :
	physics_(physics),
	player_(nullptr)
{
	player_ = std::unique_ptr<GameLogicObject>(new GameLogicObject(this, { .0f, .0f }, true));
}

int GameLogic::update()
{
	return 0;
}

void GameLogic::init()
{
	
}

int GameLogic::onBodiesCollision(const PhysicsBody & body1, const PhysicsBody & body2)
{
	// if any of the objects is player - game is over
	if (body1.getLogicObject()->isPlayer() || body2.getLogicObject()->isPlayer())
		return 1;

	return 0;
}
