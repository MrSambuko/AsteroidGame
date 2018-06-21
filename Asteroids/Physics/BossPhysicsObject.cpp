#include "SFML/Graphics/CircleShape.hpp"

#include "BossPhysicsObject.hpp"

BossPhysicsObject::BossPhysicsObject( Physics* physics, GameLogicObject* logicObject, const sf::Vector2f& position) :
	PhysicsObject(physics, logicObject, position, PY::KEEP)
{
	shape_ = std::make_shared<sf::CircleShape>(15.f, 3);
	shape_->rotate(180.f);
	shape_->setOrigin(shape_->getLocalBounds().width/2, shape_->getLocalBounds().height/2);
	shape_->setOutlineThickness(3.f);
	shape_->setOutlineColor(sf::Color::Red);
	shape_->setFillColor(sf::Color::Black);
}
