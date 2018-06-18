#include <assert.h>
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
	auto elapsed = clock_.getElapsedTime();

	int curAsteroids = gameLogic_->getNumOfAsteroids();

	if (curAsteroids < currentScenario_.maxAsteroids)
		gameLogic_->createGameObject({ .0f, .0f }, ASTEROID);
}

ScenarioDetails Scenario::getScenarioDetails(int level)
{
	assert(scenarios_.find(level) != scenarios_.end());
	return scenarios_[level];
}
