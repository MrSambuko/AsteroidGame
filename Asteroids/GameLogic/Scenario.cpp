#include <cassert>
#include <fstream>
#include <regex>
#include <string>

#include <iostream>

#include "GameLogic.hpp"

#include "Scenario.hpp"

Scenario::Scenario(const std::string& fileName) : gameLogic_(nullptr)
{
	scenarios_[0] = { 1000, 600, 2, 10, 0, 0 };
	/*
	std::ifstream file;
	file.open(fileName);

	std::regex level("\\[level\\s*(\\d+)\\]");
	std::smatch match;
	std::string line;

	while (std::getline(file, line))
	{
		while (std::regex_search(line, match, level))
		{
			for (auto x : match)
				std::cout << x << " ";
		}
	}*/
}

void Scenario::update(const sf::Time& dt)
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
	gameLogic_->createGameObject({ .0f, .0f }, GL::ASTEROID);
	asteroidSpawnTimer_.restart();
}
