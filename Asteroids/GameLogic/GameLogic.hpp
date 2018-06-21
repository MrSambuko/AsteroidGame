#pragma once

#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "SFML/Window/Keyboard.hpp"
#include "SFML/Window/Mouse.hpp"

#include "GameLogicObject.hpp"
#include "Scenario.hpp"
#include "PlayerGameLogicObject.hpp"


namespace sf
{
class Window;
}

class Physics;


static std::unordered_map< GL::GameLogicObjectType, PY::LeaveFieldStrategy> LeaveLogicTypeToStrategy = 
{
	{GL::PLAYER, PY::KEEP},
	{GL::ASTEROID, PY::DESTROY},
	{GL::PROJECTILE, PY::DESTROY},
	{GL::BOSS, PY::KEEP}
};

enum GameLogicState
{
	PLAYING,
	LEVEL_COMPLETE,
	GAME_OVER
};

class GameLogic final
{
public:
	explicit GameLogic(sf::Window* window, Scenario&& scenario, Physics* physics);

	void init(int level);
	GameLogicState update( float dt );

	void handleKeyPressed(sf::Keyboard::Key key) const;
	void handleKeyReleased(sf::Keyboard::Key key) const;
	void handleMousePressedEvent(sf::Mouse::Button button) const;
	void handleMouseReleasedEvent(sf::Mouse::Button button) const;

	void createGameObject(const sf::Vector2f& position, GL::GameLogicObjectType type);
	Physics* getPhysics() const { return physics_; }

	constexpr int getNumOfAsteroids() const { return numAsteroids_; }
	constexpr int getNumOfBosses() const { return numBosses_; }
	constexpr int getScore() const { return score_; }

	const std::unordered_set<GameLogicObjectPtr>& getObjects() const { return objects_; }

private:
	void movePlayer() const;
	void rotatePlayer() const;
	void handlePlayerShooting();
	void destroyObjects();

	void onBodiesCollision(PhysicsObject& body1, PhysicsObject& body2);

private:
	std::shared_ptr<PlayerGameLogicObject> player_ = nullptr;

	std::unordered_set<GameLogicObjectPtr> objects_;

	float playerMoveSpeed_ = 0.0f;
	float asteroidMoveSpeed_ = 0.0f;
	float projectileSpeed_ = 0.0f;

	int score_ = 0;
	int numAsteroids_ = 0;
	int numBosses_ = 0;

	int asteroidReward_ = 0;
	int bossReward_ = 0;
	float shootInterval_ = 0.0f;

	sf::Window* window_ = nullptr;
	Scenario scenario_;
	ScenarioDetails currentScenario_;
	Physics* physics_ = nullptr;

};