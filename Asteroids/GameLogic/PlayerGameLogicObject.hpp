#pragma once

#include <chrono>


class PlayerGameLogicObject : public GameLogicObject
{
public:
	PlayerGameLogicObject(GameLogic* logic) : GameLogicObject(logic, {}, GL::PLAYER) {}
	
	constexpr void startMoving(GL::MoveDirection direction)
	{
		moveDirection_ |= direction;
	}

	constexpr void stopMoving(GL::MoveDirection direction)
	{
		moveDirection_ ^= direction;
	}

	constexpr void startShooting()
	{
		isShooting_ = true;
	}

	constexpr void stopShooting()
	{
		isShooting_ = false;
	}

	constexpr void setLastTimeShooting(const std::chrono::time_point<std::chrono::steady_clock>& lastTimeShooting)
	{
		lastTimeShooting_ = lastTimeShooting;
	}

	constexpr bool isMoving() const { return moveDirection_ != GL::NO_MOVE; }
	constexpr bool isShooting() const { return isShooting_; }
	constexpr std::chrono::time_point<std::chrono::steady_clock> lastTimeShooting() const { return lastTimeShooting_; }
	constexpr GL::MoveDirection getMoveDirection() const { return moveDirection_; }

private:
	bool isShooting_ = false;
	std::chrono::time_point<std::chrono::steady_clock> lastTimeShooting_;
	GL::MoveDirection moveDirection_ = GL::NO_MOVE;
};

