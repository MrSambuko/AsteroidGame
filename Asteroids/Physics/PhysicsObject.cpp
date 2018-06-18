#include "GameLogic\GameLogicObject.hpp"
#include "Physics.hpp"

#include "PhysicsObject.hpp"



PhysicsObject::PhysicsObject(Physics* physics, GameLogicObject* logicObject, const sf::Vector2f&& position, LeaveFieldStrategy strategy) :
	physics_(physics),
	logicObject_(logicObject),
	strategy_(strategy),
	position_(position)
{
	shape_.setPosition(position);
	shape_.setRadius(10);
}

void PhysicsObject::move()
{
	position_ += velocity_;

	const auto& bounds = physics_->getBounds();
	if (position_.x < 0 || position_.y < 0 || position_.x > bounds.x || position_.y > bounds.y)
	{
		switch (strategy_)
		{
		case DESTROY:
			logicObject_->markForDestruction();
			break;

		case BOUNCE:
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
	return shape_.getGlobalBounds().intersects(other.shape_.getGlobalBounds());
}