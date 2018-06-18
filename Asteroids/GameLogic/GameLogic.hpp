#pragma once

#include <memory>
#include <unordered_set>
#include <unordered_map>

#include "SFML\System\Time.hpp"
#include "SFML\System\Vector2.hpp"

#include "GameLogicObject.hpp"
#include "Scenario.hpp"

class Physics;

static std::unordered_map<GameLogicObjectType, LeaveFieldStrategy> LogicTypeToStrategy = 
{
	{PLAYER, KEEP},
	{ASTEROID, DESTROY},
	{PROJECTILE, DESTROY},
	{BOSS, KEEP}
};



class GameLogic final
{
public:
	explicit GameLogic(const Scenario& scenario, Physics* physics);

	void init();
	int update(const sf::Time& dt);

	void createGameObject(const sf::Vector2f& position, GameLogicObjectType type);
	Physics* getPhysics() const { return physics_; }

	constexpr int getNumOfAsteroids() const { return numAsteroids_; }
	constexpr int getNumOfBosses() const { return numBosses_; }
	constexpr int getScore() const { return score_; }

private:
	void destroyObjects();

	void onBodiesCollision(const PhysicsObject& body1, const PhysicsObject& body2);

private:
	GameLogicObjectPtr player_;

	std::unordered_set<GameLogicObjectPtr> objects_;
	int score_;
	int numAsteroids_;
	int numBosses_;

	Scenario scenario_;
	Physics* physics_;

};