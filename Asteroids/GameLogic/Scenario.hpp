#pragma once

#include <string>
#include <unordered_map>

#include "SFML\System\Clock.hpp"
#include "SFML\System\Time.hpp"

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
	explicit Scenario(const std::string& filename);

	void start() { clock_.restart(); }
	void update(const sf::Time& dt);

	void setGameLogic(GameLogic* gameLogic) { gameLogic_ = gameLogic; }
	void setCurrentLevel(int level) { currentScenario_ = scenarios_[level]; }
	ScenarioDetails getScenarioDetails(int level);

private:
	sf::Clock clock_;
	GameLogic* gameLogic_;

	std::unordered_map<int, ScenarioDetails> scenarios_;
	ScenarioDetails currentScenario_;
};