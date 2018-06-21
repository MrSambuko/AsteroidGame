#include <cassert>
#include <regex>
#include <string>

#include "System/FileReader.hpp"

#include "GameLogic.hpp"

#include "Scenario.hpp"


static const std::regex levelPattern(R"(\w+(\d+))");



Scenario::Scenario(const std::string& fileName) : gameLogic_(nullptr)
{
	FileReader::Sections levels = FileReader::readIniFile(fileName);
	std::smatch match;

	for (auto&& levelDescription : levels)
	{
		assert(std::regex_search(levelDescription.first, match, levelPattern));
		const int level = std::stoi(match[1])-1;

		std::unordered_map<std::string, int> p = 
		{
			{"targetScore", 0},
			{"time", 0},
			{"maxAsteroids", 0},
			{"asteroidsInterval", 0},
			{"maxBosses", 0},
			{"bossInterval", 0},
		};

		scenarios_[level] = { levelDescription.second["targetScore"], 
							  levelDescription.second["time"], 
							  levelDescription.second["maxAsteroids"], 
							  levelDescription.second["asteroidsInterval"], 
							  levelDescription.second["maxBosses"], 
							  levelDescription.second["bossInterval"]};
	}
}

void Scenario::update( float dt )
{
	const int curAsteroids = gameLogic_->getNumOfAsteroids();

	if (curAsteroids == 0)
	{
		spawnAsteroid();
	}
	else if (curAsteroids < currentScenario_.maxAsteroids)
	{
		const auto& elapsed = asteroidSpawnTimer_.getElapsedTime().asSeconds();
		if (elapsed > currentScenario_.asteroidsInterval)
			spawnAsteroid();
	}

	if (gameLogic_->getNumOfBosses() < currentScenario_.maxBosses)
	{
		const auto& elapsed = bossSpawnTimer_.getElapsedTime().asSeconds();

		if (elapsed > currentScenario_.bossInterval)
		{
			gameLogic_->createGameObject({.0f, .0f}, GL::BOSS);
			bossSpawnTimer_.restart();
		}
	}
}

ScenarioDetails Scenario::getScenarioDetails(int level)
{
	assert(scenarios_.find(level) != scenarios_.end());
	return scenarios_[level];
}

void Scenario::spawnAsteroid()
{
	gameLogic_->createGameObject({}, GL::ASTEROID);
	asteroidSpawnTimer_.restart();
}
