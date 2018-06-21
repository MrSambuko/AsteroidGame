#include <memory>

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/CircleShape.hpp"

#include "Physics.hpp"

#include "PlayerPhysicsObject.hpp"

PlayerPhysicsObject::PlayerPhysicsObject( Physics* physics, GameLogicObject* logicObject) :
	PhysicsObject(physics, logicObject, {physics->getBounds().x/2, physics->getBounds().y/2}, PY::KEEP)
{
	shape_ = std::make_shared<sf::CircleShape>(10.f, 3);
	shape_->rotate(180.f);
	shape_->setOrigin(shape_->getLocalBounds().width/2, shape_->getLocalBounds().height/2);
	shape_->setOutlineThickness(3.f);
	shape_->setOutlineColor(sf::Color::Blue);
	shape_->setFillColor(sf::Color::Black);
}

