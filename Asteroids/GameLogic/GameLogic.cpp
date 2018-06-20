#include "Physics/Physics.hpp"

#include "GameLogic.hpp"


GameLogic::GameLogic(sf::Window* window, Scenario&& scenario, Physics* physics) :
	window_(window),
	scenario_(scenario),
	physics_(physics)
{
}

int GameLogic::update( float dt )
{
	// move player
	rotatePlayer();
	movePlayer();

	// shoot
	handlePlayerShooting();

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

	case sf::Keyboard::Space:
		player_->startShooting();
		break;

	default: 
		break;
	}
}

void GameLogic::handleKeyReleased( sf::Keyboard::Key key ) const
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

	case sf::Keyboard::Space:
		player_->stopShooting();
		break;

	default: 
		break;
	}
}

void GameLogic::handleMousePressedEvent(sf::Mouse::Button button) const
{
	player_->startShooting();
}

void GameLogic::handleMouseReleasedEvent(sf::Mouse::Button button) const
{
	player_->stopShooting();
}

void GameLogic::createGameObject(const sf::Vector2f & position, GL::GameLogicObjectType type)
{
	switch (type)
	{
	case GL::ASTEROID:
		objects_.emplace(std::make_shared<AsteroidGameLogicObject>(this));
		++numAsteroids_;
		break;
	case GL::BOSS:
		++numBosses_;
		break;
	case GL::PROJECTILE:
	{
		const auto& direction = player_->getPhysicalObject()->getDirection();
		const auto& newPosition = position + direction*11.f;
		objects_.emplace(std::make_shared<ProjectileGameLogicObject>(this, position, player_->getPhysicalObject()->getDirection()));
		break;
	}
	default:
		break;
	};
}


void GameLogic::init()
{
	physics_->setCollistionCallback(
		[this](PhysicsObject& body1, PhysicsObject& body2)
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
		switch (object->getType())
		{
		case GL::ASTEROID:
			--numAsteroids_;
			break;
		case GL::PLAYER:
			player_ = nullptr;
			break;

		default:
			break;
		}
		objectsToDestroy.pop_back();
		objects_.erase(object);
	}
}

void GameLogic::movePlayer() const
{
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

}

void GameLogic::rotatePlayer() const
{
	const auto& player = player_->getPhysicalObject();
	const auto& mousePosition = sf::Mouse::getPosition(*window_);
	const sf::Vector2f localPosition(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
	const auto& direction = localPosition - player->getPosition();
	player->setDirection(std::move(direction));
}

void GameLogic::handlePlayerShooting()
{
	if (player_->isShooting())
	{
		const auto& now = std::chrono::steady_clock::now();
		const auto& diffTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - player_->lastTimeShooting());
		if (diffTime.count() > GL::SHOOT_INTERVAL)
		{
			createGameObject(player_->getPhysicalObject()->getPosition(), GL::PROJECTILE);
			player_->setLastTimeShooting(now);
		}
	}
}

void GameLogic::onBodiesCollision(PhysicsObject& body1, PhysicsObject& body2) const
{
	// if any of the objects is player - game is over
	const auto& b1Type = body1.getLogicObject()->getType();
	const auto& b2Type = body2.getLogicObject()->getType();
	if (b1Type == GL::PLAYER || b2Type == GL::PLAYER)
	{
		// ignore collision with own bullets
		if (b1Type != GL::PROJECTILE && b2Type != GL::PROJECTILE)
			player_->markForDestruction();
	}
	else
	{
		if ((b1Type == GL::PROJECTILE && b2Type == GL::ASTEROID) || (b1Type == GL::ASTEROID && b2Type == GL::PROJECTILE))
		{
			body1.getLogicObject()->markForDestruction();
			body2.getLogicObject()->markForDestruction();
		}
		else
		{
			body1.reverseVelocity();
			body2.reverseVelocity();
		}
	}
	return;
}
