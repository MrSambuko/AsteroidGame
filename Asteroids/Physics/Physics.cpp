#include "Physics.hpp"



PhysicsBody::PhysicsBody(GameLogicObject* logicObject, const sf::Vector2f&& position) :
	logicObject_(logicObject),
	position_(position)
{
	shape_.setPosition(position);
	shape_.setRadius(10);
}

bool PhysicsBody::intersects(const PhysicsBody& other) const
{
	return shape_.getGlobalBounds().intersects(other.shape_.getGlobalBounds());
}

PhysicsBody* Physics::createPhysicsBody(GameLogicObject* obj, const sf::Vector2f& position)
{
	auto ptr = std::make_shared<PhysicsBody>( obj, std::move(position) );
	bodies_.push_back(ptr);
	return ptr.get();
}

int Physics::update()
{
	for (auto i = 0; i < bodies_.size(); ++i)
	{
		for (auto j = i; j < bodies_.size(); ++j)
		{
			if (bodies_[i]->intersects(*bodies_[j]))
				callback_(*bodies_[i], *bodies_[j]);
		}
	}
	return 0;
}

