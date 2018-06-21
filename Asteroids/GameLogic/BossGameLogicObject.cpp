#include "Physics/Physics.hpp"
#include "System/Math.hpp"

#include "GameLogic.hpp"

#include "BossGameLogicObject.hpp"



BossGameLogicObject::BossGameLogicObject( GameLogic* logic, const sf::Vector2f& position):
	GameLogicObject(logic, logic->getPhysics()->generateRandomPositionOutsideBounds(), GL::BOSS)
{
	const auto& playerPosition = gameLogic_->getPlayer().getPhysicalObject()->getPosition();
	const auto& bossPosition = getPhysicalObject()->getPosition();
	const auto& newDirection = normalizeVector(playerPosition-bossPosition);
	physicsObject_->setDirection(newDirection);
}
