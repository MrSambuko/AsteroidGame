#pragma once

#include <string>
#include <unordered_map>

#include "SFML/System/Clock.hpp"

class GameLogic;

struct ScenarioDetails
{
	int targetScore = 0;
	int time = 0;
	int maxAsteroids = 0;
	int asteroidsInterval = 0;
	int maxBosses = 0;
	int bossInterval = 0;
};

class Scenario final
{
public:
	explicit Scenario(const std::string& fileName);

	void start() { asteroidSpawnTimer_.restart(); }
	void update( float dt );

	void setGameLogic(GameLogic* gameLogic) { gameLogic_ = gameLogic; }
	void setCurrentLevel(int level) { currentScenario_ = scenarios_[level]; }
	ScenarioDetails getScenarioDetails(int level);

private:
	void spawnAsteroid();

private:
	sf::Clock asteroidSpawnTimer_;
	sf::Clock bossSpawnTimer_;
	GameLogic* gameLogic_;

	std::unordered_map<int, ScenarioDetails> scenarios_;
	ScenarioDetails currentScenario_;
};