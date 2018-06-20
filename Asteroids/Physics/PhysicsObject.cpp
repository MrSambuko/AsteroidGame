#include "SFML/Graphics/CircleShape.hpp"

#include "GameLogic/GameLogicObject.hpp"
#include "Physics.hpp"

#include "PhysicsObject.hpp"


PhysicsObject::PhysicsObject(Physics* physics, GameLogicObject* logicObject, const sf::Vector2f& position, PY::LeaveFieldStrategy strategy) :
	physics_(physics),
	logicObject_(logicObject),
	strategy_(strategy),
	position_(position),
	shape_(nullptr)
{
}

void PhysicsObject::move(const float& dt)
{
	position_ += velocity_ * dt;

	const auto& bounds = physics_->getBounds();
	if (position_.x < 0 || position_.y < 0 || position_.x > bounds.x || position_.y > bounds.y)
	{
		switch (strategy_)
		{
		case PY::DESTROY:
			logicObject_->markForDestruction();
			break;

		case PY::BOUNCE:
			if (position_.x < 0 || position_.x > bounds.x)
				velocity_.x = -velocity_.x;
			if (position_.y < 0 || position_.y > bounds.y)
				velocity_.y = -velocity_.y;
			break;

		default:
			break;
		}
	}

	shape_->setPosition(position_);
}

void PhysicsObject::reverseVelocity()
{
	velocity_.x = -velocity_.x;
	velocity_.y = -velocity_.y;
}


void PhysicsObject::setDirection( const sf::Vector2f&& newDirection )
{
	direction_ = Physics::normalizeVector(std::move(newDirection));
}

bool PhysicsObject::intersects(const PhysicsObject& other) const
{
	return shape_->getGlobalBounds().intersects(other.shape_->getGlobalBounds());
}

PlayerPhysicsObject::PlayerPhysicsObject( Physics* physics, GameLogicObject* logicObject) :
	PhysicsObject(physics, logicObject, {physics->getBounds().x/2, physics->getBounds().y/2}, PY::KEEP)
{
	shape_ = std::make_shared<sf::CircleShape>(10.f, 256);
	shape_->setFillColor(sf::Color::Red);
}


AsteroidPhysicsObject::AsteroidPhysicsObject( Physics* physics, GameLogicObject* logicObject, const sf::Vector2f& position ) :
	PhysicsObject(physics, logicObject, position, PY::BOUNCE)
{
	shape_ = std::make_shared<sf::CircleShape>(25.f, 256);
}

ProjectilePhysicsObject::ProjectilePhysicsObject( Physics* physics, GameLogicObject* logicObject,const sf::Vector2f& position ) :
	PhysicsObject(physics, logicObject, position, PY::DESTROY)
{
	shape_ = std::make_shared<sf::CircleShape>(2.5f, 32);
	shape_->setFillColor(sf::Color::Yellow);

}
