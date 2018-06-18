
#include "Physics\Physics.hpp"

#include "GameLogic.hpp"




GameLogic::GameLogic(Physics* physics) :
	player_(nullptr),
	physics_(physics)
{
}

int GameLogic::update()
{
	destroyObjects();
	return 0;
}

void GameLogic::createGameObject(const sf::Vector2f & position, GameLogicObjectType type)
{
	objects_.insert(std::make_shared<GameLogicObject>(this, position, type));
}

void GameLogic::init()
{
	physics_->setCollistionCallback(
		[this](const PhysicsObject& body1, const PhysicsObject& body2)
		{
			this->onBodiesCollision(body1, body2);
		});
	player_ = std::make_shared<GameLogicObject>(this, sf::Vector2f(.0f, .0f), PLAYER);
	objects_.insert(player_);
}

void GameLogic::destroyObjects()
{
	std::vector<GameLogicObjectPtr> objectsToDestroy;
	for (const auto& object : objects_)
	{
		if (object->shouldDestroy())
		{
			objectsToDestroy.push_back(object);
		}
	}
	while (!objectsToDestroy.empty())
	{
		const auto object = objectsToDestroy.back();
		objectsToDestroy.pop_back();
		objects_.erase(object);
	}
}

void GameLogic::onBodiesCollision(const PhysicsObject& body1, const PhysicsObject& body2)
{
	// if any of the objects is player - game is over
	if (body1.getLogicObject()->getType() == PLAYER || body2.getLogicObject()->getType() == PLAYER)
	{
		player_->markForDestruction();
		player_ = nullptr;
	}
	return;
}
