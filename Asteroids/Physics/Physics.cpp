#include "GameLogic/GameLogic.hpp"

#include "Physics.hpp"



PhysicsBody::PhysicsBody(Physics* physics, GameLogicObject* logicObject, const sf::Vector2f&& position, LeaveFieldStrategy strategy) :
	physics_(physics),
	logicObject_(logicObject),
	strategy_(strategy),
	position_(position)
{
	shape_.setPosition(position);
	shape_.setRadius(10);
}

void PhysicsBody::move()
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

void PhysicsBody::reverseVelocity()
{
	velocity_.x = -velocity_.x;
	velocity_.y = -velocity_.y;
}

bool PhysicsBody::intersects(const PhysicsBody& other) const
{
	return shape_.getGlobalBounds().intersects(other.shape_.getGlobalBounds());
}

PhysicsBodyPtr Physics::createPhysicsBody(GameLogicObject* obj, const sf::Vector2f& position, LeaveFieldStrategy strategy)
{
	auto ptr = std::make_shared<PhysicsBody>( this, obj, std::move(position), strategy );
	bodies_.insert(ptr);
	return ptr;
}

void Physics::destroyPhysicsBody( const PhysicsBodyPtr body )
{
	bodiesToDestroy_.push_back(body);
}

int Physics::update()
{
	updatePositions();
	updateCollisions();

	while (!bodiesToDestroy_.empty())
	{
		const auto body = bodiesToDestroy_.back();
		bodiesToDestroy_.pop_back();
		bodies_.erase(body);
	}
	bodiesToDestroy_.clear();

	return 0;
}

void Physics::updateCollisions()
{
	std::vector<PhysicsBodyPtr> bodies(bodies_.begin(), bodies_.end());
	const auto s = bodies.size();

	for (size_t i = 0; i < s; ++i)
	{
		for (size_t j = i+1; j < s; ++j)
		{
			auto& b1 = *bodies[i];
			auto& b2 = *bodies[j];
			if (b1.intersects(b2))
			{
				b1.reverseVelocity();
				b2.reverseVelocity();
				callback_(b1, b2);
			}
		}
	}
}

void Physics::updatePositions()
{
	for (const auto& body : bodies_)
		body->move();
}

