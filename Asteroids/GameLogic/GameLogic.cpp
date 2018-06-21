#include <cassert>
#include <string>

#include "Physics/Physics.hpp"
#include "System/FileReader.hpp"
#include "System/Math.hpp"

#include "AsteroidGameLogicObject.hpp"
#include "GameLogic.hpp"
#include "ProjectileGameLogicObject.hpp"

#include "BossGameLogicObject.hpp"


static const std::vector<std::string> SETTINGS = {
	"playerMoveSpeed",
	"asteroidMoveSpeed",
	"asteroidReward",
	"bossReward",
	"asteroidSplitNumber",
	"shootInterval",
	"projectileMoveSpeed",
	"bossMoveSpeed",
	"smallAsteroidReward",
};

static std::unordered_map<sf::Keyboard::Key, int> keyToDirectionMap =
{
{ sf::Keyboard::W, GL::UP },
{ sf::Keyboard::D, GL::RIGHT },
{ sf::Keyboard::S, GL::DOWN },
{ sf::Keyboard::A, GL::LEFT },
};

static std::unordered_map<int, sf::Vector2f> PLAYER_MOVE_SPEED =
{
{ GL::NO_MOVE,{} },
{ GL::UP,	 sf::Vector2f(.0f, -1.0f)},
{ GL::RIGHT, sf::Vector2f(1.0f,   .0f) },
{ GL::DOWN,	 sf::Vector2f(.0f,  1.0f) },
{ GL::LEFT,	 sf::Vector2f(-1.0f,  .0f) }
};

GameLogic::GameLogic(sf::Window* window, Scenario&& scenario, Physics* physics) :
	window_(window),
	scenario_(scenario),
	physics_(physics)
{
	auto& sections = FileReader::readIniFile("resources/game.ini");
	assert(sections.find("gameplay") != sections.end());

	auto& gameplaySettings = sections["gameplay"];

	for (const auto& setting : SETTINGS)
		assert(gameplaySettings.find(setting) != gameplaySettings.end());

	numAsteroids_ = gameplaySettings["numAsteroids"];
	asteroidReward_ = gameplaySettings["asteroidReward"];
	bossReward_ = gameplaySettings["bossReward"];
	shootInterval_ = static_cast<float>(gameplaySettings["shootInterval"]);
	playerMoveSpeed_ = static_cast<float>(gameplaySettings["playerMoveSpeed"]);
	asteroidMoveSpeed_ = static_cast<float>(gameplaySettings["asteroidMoveSpeed"]);
	projectileSpeed_ = static_cast<float>(gameplaySettings["projectileMoveSpeed"]);
	bossMoveSpeed_ = static_cast<float>(gameplaySettings["bossMoveSpeed"]);
	smallAsteroidReward_ = gameplaySettings["smallAsteroidReward"];
}

GameLogicState GameLogic::update( float dt )
{
	if (score_ >= currentScenario_.targetScore)
		return LEVEL_COMPLETE;

	destroyObjects();
	if (player_ == nullptr)
		return GAME_OVER;

	rotatePlayer();
	movePlayer();
	handlePlayerShooting();
	
	// bosses
	moveBosses();
	handleBossesShooting();

	scenario_.update(dt);
	
	return PLAYING;
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
		const auto& direction = keyToDirectionMap[key];
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
		if (!player_->isMoving())
			break;

		const auto& direction = keyToDirectionMap[key];
		player_->stopMoving(direction);
		break;
	}

	case sf::Keyboard::Space:
		if (!player_->isShooting())
			break;
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

void GameLogic::createGameObject( const sf::Vector2f& position, GL::GameLogicObjectType type, GameLogicObjectPtr owner )
{
	switch (type)
	{
	case GL::ASTEROID:
		objects_.emplace(std::make_shared<AsteroidGameLogicObject>(this, asteroidMoveSpeed_));
		++numAsteroids_;
		break;

	case GL::SMALL_ASTEROID:
	{
		const auto& pObject = owner->getPhysicalObject();
		const auto& ownerPosition = pObject->getPosition();
		const auto& direction = normalizeVector(pObject->getVelocity());
		const sf::Vector2f direction1(-direction.y,  direction.x);
		const sf::Vector2f position1 = ownerPosition + direction1*60.f;
		const sf::Vector2f direction2( direction.y, -direction.x);
		const sf::Vector2f position2 = ownerPosition + direction2*60.f;

		objects_.emplace(std::make_shared<SmallAsteroidGameLogicObject>(this, position1, direction1, asteroidMoveSpeed_*1.5f));
		objects_.emplace(std::make_shared<SmallAsteroidGameLogicObject>(this, position2, direction2, asteroidMoveSpeed_*1.5f));
		break;
	}

	case GL::BOSS:
	{
		const auto boss = std::make_shared<BossGameLogicObject>(this, position);
		objects_.insert(boss);
		bosses_.emplace(boss);

		++numBosses_;
		break;
	}
	case GL::PLAYER_PROJECTILE:
		objects_.emplace(std::make_shared<ProjectileGameLogicObject>(
			this, 
			position, 
			player_->getPhysicalObject()->getDirection(), 
			projectileSpeed_));
		break;
	case GL::BOSS_PROJECTILE:
		objects_.emplace(std::make_shared<BossProjectileGameLogicObject>(
			this,
			position,
			owner->getPhysicalObject()->getDirection(),
			projectileSpeed_));
		break;
	default:
		break;
	};
}


void GameLogic::init(int level)
{
	physics_->setCollistionCallback(
		[this](PhysicsObject& body1, PhysicsObject& body2)
		{
			this->onBodiesCollision(body1, body2);
		});
	player_ = std::make_shared<PlayerGameLogicObject>(this);
	objects_.insert(player_);
	scenario_.setGameLogic(this);
	scenario_.setCurrentLevel(level);
	currentScenario_ = scenario_.getScenarioDetails(level);
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
			createGameObject(object->getPhysicalObject()->getPosition(), GL::SMALL_ASTEROID, object);
			break;
		case GL::PLAYER:
			player_ = nullptr;
			break;
		case GL::BOSS:
			bosses_.erase(std::static_pointer_cast<BossGameLogicObject>(object));
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
		for (const auto& pair : PLAYER_MOVE_SPEED)
		{
			if (playerMoveDirection & pair.first)
				velocity += (playerMoveSpeed_ * pair.second);
		}
		
	}
	player_->getPhysicalObject()->setVelocity(std::move(velocity));

}

void GameLogic::moveBosses() const
{
	const sf::Vector2f& playerPosition = player_->getPhysicalObject()->getPosition();

	for (auto& boss : bosses_)
	{
		auto pObject = boss->getPhysicalObject();
		const auto& direction = normalizeVector(playerPosition - pObject->getPosition());
		pObject->setDirection(direction);
		pObject->setVelocity(direction*bossMoveSpeed_);
	}
}

void GameLogic::rotatePlayer() const
{
	const auto& player = player_->getPhysicalObject();
	const auto& mousePosition = sf::Mouse::getPosition(*window_);
	const sf::Vector2f localPosition(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));
	const auto& direction = normalizeVector(localPosition - player->getPosition());
	player->setDirection(direction);
}

void GameLogic::handlePlayerShooting()
{
	if (player_->isShooting())
	{
		const auto& now = std::chrono::steady_clock::now();
		const auto& diffTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - player_->lastTimeShooting());
		if (diffTime.count() > shootInterval_)
		{
			const auto& pObject = player_->getPhysicalObject();

			createGameObject(pObject->getPosition()+pObject->getDirection(), GL::PLAYER_PROJECTILE);
			player_->setLastTimeShooting(now);
		}
	}
}

void GameLogic::handleBossesShooting()
{
	for (auto& boss : bosses_)
	{
		const auto& now = std::chrono::steady_clock::now();
		const auto& diffTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - boss->lastTimeShooting());
		if (diffTime.count() > shootInterval_)
		{
			const auto& pObject = boss->getPhysicalObject();

			createGameObject(pObject->getPosition()+pObject->getDirection(), GL::BOSS_PROJECTILE, boss);
			boss->setLastTimeShooting(now);
		}
	}
}

void GameLogic::onBodiesCollision(PhysicsObject& body1, PhysicsObject& body2)
{
	// if any of the objects is player - game is over
	const auto& b1Type = body1.getLogicObject()->getType();
	const auto& b2Type = body2.getLogicObject()->getType();

	const bool& b1IsPlayer = b1Type == GL::PLAYER;
	const bool& b2IsPlayer = b2Type == GL::PLAYER;

	const bool& b1IsBoss = b1Type == GL::BOSS;
	const bool& b2IsBoss = b2Type == GL::BOSS;

	const bool& b1IsBigAsteroid = b1Type == GL::ASTEROID;
	const bool& b1IsSmallAsteroid = b1Type == GL::SMALL_ASTEROID;
	const bool& b1IsAsteroid = b1IsBigAsteroid || b1IsSmallAsteroid;
	
	const bool& b2IsBigAsteroid = b2Type == GL::ASTEROID;
	const bool& b2IsSmallAsteroid = b2Type == GL::SMALL_ASTEROID;
	const bool& b2IsAsteroid = b2IsBigAsteroid || b2IsSmallAsteroid;

	const bool& b1IsPlayerProjectile = b1Type ==  GL::PLAYER_PROJECTILE;
	const bool& b1IsBossProjectile = b1Type == GL::BOSS_PROJECTILE;
	const bool& b1IsProjectile = b1IsPlayerProjectile || b1IsBossProjectile;

	const bool& b2IsPlayerProjectile = b2Type ==  GL::PLAYER_PROJECTILE;
	const bool& b2IsBossProjectile = b2Type == GL::BOSS_PROJECTILE;
	const bool& b2IsProjectile = b2IsPlayerProjectile || b2IsBossProjectile;

	
	if (b1IsPlayer || b2IsPlayer)
	{
		// ignore collision with own bullets
		if (!b1IsPlayerProjectile && !b2IsPlayerProjectile)
		{
			player_->markForDestruction();
		}
	}
	else
	{
		// if bullet hits asteroid - destroy it
		if ((b1IsProjectile && b2IsAsteroid) || (b1IsAsteroid && b2IsProjectile))
		{
			body1.getLogicObject()->markForDestruction();
			body2.getLogicObject()->markForDestruction();

			// only credit big asteroids
			if (b1IsBigAsteroid || b2IsBigAsteroid)
			{
				scenario_.asteroidDestroyed();

				score_ += asteroidReward_;
			}
			else
			{
				score_ += smallAsteroidReward_;
			}
		}
		// destroy asteroids that are hit by boss
		else if ((b1IsBoss && b2IsAsteroid) || (b1IsAsteroid && b2IsBoss))
		{
			if (b1IsAsteroid)
				body1.getLogicObject()->markForDestruction();
			else
				body2.getLogicObject()->markForDestruction();
		}
		// boss ignore it's own bullets
		else if ((b1IsBoss && b2IsBossProjectile) || (b1IsBossProjectile && b2IsBoss))
			return;
		// kill bosses
		else if ((b1IsBoss && b2IsPlayerProjectile) || (b1IsPlayerProjectile && b2IsBoss))
		{
			body1.getLogicObject()->markForDestruction();
			body2.getLogicObject()->markForDestruction();

			score_ += bossReward_;
		}
		else
		{
			body1.reverseVelocity();
			body2.reverseVelocity();
		}
	}
	return;
}
