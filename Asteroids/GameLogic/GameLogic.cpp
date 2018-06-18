
#include "Physics\Physics.hpp"

#include "GameLogic.hpp"



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


GameLogic::GameLogic(Physics* physics) :
	player_(nullptr),
	physics_(physics)
{
}

int GameLogic::update()
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
	return 0;
}

void GameLogic::createGameObject(const sf::Vector2f & position, GameLogicObjectType type)
{
	objects_.insert(std::make_shared<GameLogicObject>(this, position, type));
}

void GameLogic::init()
{
	physics_->setCollistionCallback([this](const PhysicsBody& body1, const PhysicsBody& body2){this->onBodiesCollision(body1, body2);});
	player_ = std::make_shared<GameLogicObject>(this, sf::Vector2f(.0f, .0f), PLAYER);
	objects_.insert(player_);
}

void GameLogic::onBodiesCollision(const PhysicsBody& body1, const PhysicsBody& body2)
{
	// if any of the objects is player - game is over
	if (body1.getLogicObject()->getType() == PLAYER || body2.getLogicObject()->getType() == PLAYER)
	{
		player_->markForDestruction();
		player_ = nullptr;
	}
	return;
}
