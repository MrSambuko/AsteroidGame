#include "ProjectileGameLogicObject.hpp"



ProjectileGameLogicObject::ProjectileGameLogicObject( GameLogic* logic, const sf::Vector2f& position, const sf::Vector2f& direction, float speed): 
	GameLogicObject(logic, position, GL::PLAYER_PROJECTILE)
{
	physicsObject_->setVelocity(direction * speed);
}

BossProjectileGameLogicObject::BossProjectileGameLogicObject( GameLogic* logic, const sf::Vector2f& position, const sf::Vector2f& direction, float speed): 
	GameLogicObject(logic, position, GL::BOSS_PROJECTILE)
{
	physicsObject_->setVelocity(direction * speed);
}
