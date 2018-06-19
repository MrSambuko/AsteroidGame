
#include "Physics\Physics.hpp"

#include "Scenario.hpp"
#include "GameLogic.hpp"




GameLogic::GameLogic(Scenario&& scenario, Physics* physics) :
	scenario_(scenario),
	physics_(physics)
{
}

int GameLogic::update( float dt )
{
	// move player
	sf::Vector2f velocity;
	if (player_->isMoving())
	{
		const auto& playerMoveDirection = player_->getMoveDirection();
		for (const auto& pair : GL::moveToVectorMap)
		{
			if (playerMoveDirection & pair.first)
				velocity += pair.second;
		}
		
	}
	player_->getPhysicalObject()->setVelocity(std::move(velocity));

	scenario_.update(dt);
	destroyObjects();
	return 0;
}

void GameLogic::handleKeyPressed( sf::Keyboard::Key key ) const
{
	switch (key)
	{
	case sf::Keyboard::A:
	case sf::Keyboard::W:
	case sf::Keyboard::D:
	case sf::Keyboard::S:
	{
		const auto& direction = GL::keyToDirectionMap[key];
		player_->startMoving(direction);
		break;
	}

	default: 
		break;
	}
}

void GameLogic::handleKeyReleased( sf::Keyboard::Key key )
{
	switch (key)
	{
	case sf::Keyboard::A:
	case sf::Keyboard::W:
	case sf::Keyboard::D:
	case sf::Keyboard::S:
	{
		const auto& direction = GL::keyToDirectionMap[key];
		player_->stopMoving(direction);
		break;
	}

	default: 
		break;
	}
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
	player_ = std::make_shared<PlayerGameLogicObject>(this);
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
	}
	return;
}
