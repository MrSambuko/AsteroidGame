
#include "Physics\Physics.hpp"

#include "Scenario.hpp"
#include "GameLogic.hpp"




GameLogic::GameLogic(Scenario&& scenario, Physics* physics) :
	scenario_(scenario),
	physics_(physics)
{
}

int GameLogic::update(const sf::Time& dt)
{
	scenario_.update(dt);
	destroyObjects();
	return 0;
}

void GameLogic::createGameObject(const sf::Vector2f & position, GL::GameLogicObjectType type)
{
	switch (type)
	{
	case GL::ASTEROID:
		objects_.insert(std::make_shared<AsteroidGameLogicObject>(this));
		++numAsteroids_;
		break;
	case GL::BOSS:
		++numBosses_;
		break;
	default:
		break;
	};
}


void GameLogic::init()
{
	physics_->setCollistionCallback(
		[this](const PhysicsObject& body1, const PhysicsObject& body2)
		{
			this->onBodiesCollision(body1, body2);
		});
	player_ = std::make_shared<GameLogicObject>(this, sf::Vector2f(.0f, .0f), GL::PLAYER);
	objects_.insert(player_);
	scenario_.setGameLogic(this);
	scenario_.setCurrentLevel(0);
	scenario_.start();
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
	if (body1.getLogicObject()->getType() == GL::PLAYER || body2.getLogicObject()->getType() == GL::PLAYER)
	{
		player_->markForDestruction();
		player_ = nullptr;
	}
	return;
}
