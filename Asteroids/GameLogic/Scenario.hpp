#pragma once

#include <string>
#include <unordered_map>


struct ScnearioDetails
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

	ScnearioDetails getScenarioDetails(int level);
private:
	std::unordered_map<int, ScnearioDetails> scenarios_;
};