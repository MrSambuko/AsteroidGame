#pragma once

#include "GameLogicObject.hpp"

class BossGameLogicObject : public GameLogicObject
{
public:
	BossGameLogicObject( GameLogic* logic, const sf::Vector2f& position);

	constexpr void setLastTimeShooting(const std::chrono::time_point<std::chrono::steady_clock>& lastTimeShooting)
	{
		lastTimeShooting_ = lastTimeShooting;
	}
	constexpr std::chrono::time_point<std::chrono::steady_clock> lastTimeShooting() const { return lastTimeShooting_; }

private:
	std::chrono::time_point<std::chrono::steady_clock> lastTimeShooting_;
};