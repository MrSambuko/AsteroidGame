#include <assert.h>
#include <fstream>
#include <regex>
#include <string>

#include <iostream>

#include "Scenario.hpp"

Scenario::Scenario(const std::string& fileName)
{
	scenarios_[0] = { 1000, 600, 0, 10, 0, 0 };
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

ScnearioDetails Scenario::getScenarioDetails(int level)
{
	assert(scenarios_.find(level) != scenarios_.end());
	return scenarios_[level];
}
