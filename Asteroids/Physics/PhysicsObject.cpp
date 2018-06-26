#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/ConvexShape.hpp"

#include "GameLogic/GameLogicObject.hpp"
#include "System/Math.hpp"
#include "System/Common.hpp"

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
			{
				position_.x = position_.x < 0 ? 0 : bounds.x;
				velocity_.x = -velocity_.x;
			}
			if (position_.y < 0 || position_.y > bounds.y)
			{
				velocity_.y = -velocity_.y;
				position_.y = position_.y < 0 ? 0 : bounds.y;
			}
			break;

		case PY::KEEP:
			if (position_.x < 0 || position_.x > bounds.x)
			{
				velocity_.x = 0;
				position_.x = position_.x < 0 ? 0 : bounds.x;
			}
			if (position_.y < 0 || position_.y > bounds.y)
			{
				velocity_.y = 0;
				position_.y = position_.y < 0 ? 0 : bounds.y;
			}
			
			break;

		default:
			break;
		}
	}

	position_ += velocity_ * dt;
	shape_->setPosition(position_);
	shape_->setRotation(shape_->getRotation()+anglePerSeconds_);
}

void PhysicsObject::reverseVelocity()
{
	velocity_.x = -velocity_.x;
	velocity_.y = -velocity_.y;
}


void PhysicsObject::setDirection( const sf::Vector2f& newDirection )
{
	const auto& angle = angleBetweenVectors(-direction_, -newDirection) * 180 / PI;
	shape_->rotate(angle);
	direction_ = newDirection;
}

bool PhysicsObject::intersects(const PhysicsObject& other) const
{
	return shape_->getGlobalBounds().intersects(other.shape_->getGlobalBounds());
}

std::vector<sf::Vector3f> PhysicsObject::getNormals() const
{
	const auto& numPoints = shape_->getPointCount();
	std::vector<sf::Vector3f> normals(numPoints);
	const auto& transform = shape_->getTransform();
	auto& prevPoint = std::move(transform.transformPoint(shape_->getPoint(0)));

	for (size_t index = 1; index < numPoints; ++index)
	{
		const auto& p = shape_->getPoint(index);
		const auto& resultPoint = transform.transformPoint(p);
		const auto& normal = getNormal(resultPoint, prevPoint);
		normals[index] = {normal.x, normal.y, 0};
		prevPoint = resultPoint;
	}

	return normals;
}
