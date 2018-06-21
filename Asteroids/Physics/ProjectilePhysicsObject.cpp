#include <memory>

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/CircleShape.hpp"

#include "System/Math.hpp"

#include "ProjectilePhysicsObject.hpp"




ProjectilePhysicsObject::ProjectilePhysicsObject( Physics* physics, GameLogicObject* logicObject,const sf::Vector2f& position ) :
	PhysicsObject(physics, logicObject, position, PY::DESTROY)
{
	shape_ = std::make_shared<sf::CircleShape>(2.5f, 32);
	shape_->setFillColor(sf::Color::Yellow);
}
