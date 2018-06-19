#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>

#include "SFML\System\Time.hpp"
#include "SFML\System\Vector2.hpp"

#include "GameLogicObject.hpp"
#include "Scenario.hpp"

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
	int update(const sf::Time& dt);

	void createGameObject(const sf::Vector2f& position, GL::GameLogicObjectType type);
	Physics* getPhysics() const { return physics_; }

	constexpr int getNumOfAsteroids() const { return numAsteroids_; }
	constexpr int getNumOfBosses() const { return numBosses_; }
	constexpr int getScore() const { return score_; }

private:
	void destroyObjects();

	void onBodiesCollision(const PhysicsObject& body1, const PhysicsObject& body2);

private:
	GameLogicObjectPtr player_ = nullptr;

	std::unordered_set<GameLogicObjectPtr> objects_;
	int score_ = 0;
	int numAsteroids_ = 0;
	int numBosses_ = 0;

	Scenario scenario_;
	Physics* physics_ = nullptr;

};