#include "SFML/Graphics/ConvexShape.hpp"

#include "GameLogic\GameLogicObject.hpp"
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

void PhysicsObject::move()
{
	position_ += velocity_;

	const auto& bounds = physics_->getBounds();
	if (position_.x < 0 || position_.y < 0 || position_.x > bounds.x || position_.y > bounds.y)
	{
		switch (strategy_)
		{
		case PY::DESTROY:
			logicObject_->markForDestruction();
			break;

		case PY::BOUNCE:
			position_.x = -position_.x;
			position_.y = -position_.y;
			break;

		default:
			break;
		}
	}


}

void PhysicsObject::reverseVelocity()
{
	velocity_.x = -velocity_.x;
	velocity_.y = -velocity_.y;
}


bool PhysicsObject::intersects(const PhysicsObject& other) const
{
	return shape_->getGlobalBounds().intersects(other.shape_->getGlobalBounds());
}

PlayerPhysicsObject::PlayerPhysicsObject( Physics* physics, GameLogicObject* logicObject) :
	PhysicsObject(physics, logicObject, {.0f, .0f}, PY::KEEP)
{
	shape_ = std::make_unique<sf::ConvexShape>(3);
}

AsteroidPhysicsObject::AsteroidPhysicsObject( Physics* physics, GameLogicObject* logicObject, const sf::Vector2f& position ) :
	PhysicsObject(physics, logicObject, position, PY::DESTROY)
{
	shape_ = std::make_unique<sf::CircleShape>(5.f);
}
