#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>

#include "SFML\System\Vector2.hpp"

#include "GameLogicObject.hpp"
#include "Scenario.hpp"
#include <SFML/Window/Keyboard.hpp>

class Physics;


static std::unordered_map< GL::GameLogicObjectType, PY::LeaveFieldStrategy> LeaveLogicTypeToStrategy = 
{
	{GL::PLAYER, PY::KEEP},
	{GL::ASTEROID, PY::DESTROY},
	{GL::PROJECTILE, PY::DESTROY},
	{GL::BOSS, PY::KEEP}
};


class GameLogic final
{
public:
	explicit GameLogic(Scenario&& scenario, Physics* physics);

	void init();
	int update( float dt );

	void handleKeyPressed(sf::Keyboard::Key key) const;
	void handleKeyReleased(sf::Keyboard::Key key);

	void createGameObject(const sf::Vector2f& position, GL::GameLogicObjectType type);
	Physics* getPhysics() const { return physics_; }

	constexpr int getNumOfAsteroids() const { return numAsteroids_; }
	constexpr int getNumOfBosses() const { return numBosses_; }
	constexpr int getScore() const { return score_; }

	const std::unordered_set<GameLogicObjectPtr>& getObjects() const { return objects_; }

private:
	void destroyObjects();

	void onBodiesCollision(const PhysicsObject& body1, const PhysicsObject& body2);

private:
	std::shared_ptr<PlayerGameLogicObject> player_ = nullptr;

	std::unordered_set<GameLogicObjectPtr> objects_;
	int score_ = 0;
	int numAsteroids_ = 0;
	int numBosses_ = 0;

	Scenario scenario_;
	Physics* physics_ = nullptr;

};