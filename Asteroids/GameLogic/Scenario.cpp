#include <cassert>
#include <fstream>
#include <regex>
#include <string>

#include <iostream>

#include "GameLogic.hpp"

#include "Scenario.hpp"

static const std::regex levelRegex(R"(\[level\s*(\d+)\])");
static const std::regex paramRegex(R"((\w+)\s*=\s*(\d+))");



Scenario::Scenario(const std::string& fileName) : gameLogic_(nullptr)
{
	std::fstream file;
	file.open(fileName, std::fstream::in);
	
	std::smatch match;
	std::string line;

	while (std::getline(file, line))
	{
		if (std::regex_search(line, match, levelRegex))
		{
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

			while (std::getline(file, line) && std::regex_search(line, match, paramRegex))
			{
				if (p.find(match[1]) != p.end())
					p[match[1]] = std::stoi(match[2]);
			}
			scenarios_[level] = { p["targetScore"], 
								  p["time"], 
								  p["maxAteroids"], 
								  p["asteroidsInterval"], 
								  p["maxBosses"], 
								  p["bossInterval"]};

		}
	}
}

void Scenario::update( float dt )
{
	const int curAsteroids = gameLogic_->getNumOfAsteroids();

	if (curAsteroids == 0)
		spawnAsteroid();
	else if (curAsteroids < currentScenario_.maxAsteroids)
	{
		const auto elapsed = asteroidSpawnTimer_.getElapsedTime();
		if (elapsed.asSeconds() > currentScenario_.asteroidsInterval)
			spawnAsteroid();
	}

	if (gameLogic_->getNumOfBosses() < currentScenario_.maxBosses)
	{
		const auto elapsed = bossSpawnTimer_.getElapsedTime();

		if (elapsed.asSeconds() > currentScenario_.bossInterval)
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
