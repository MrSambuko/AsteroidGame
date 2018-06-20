#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/ConvexShape.hpp"

#include "GameLogic/GameLogicObject.hpp"
#include "Physics.hpp"

#include "PhysicsObject.hpp"
#include <random>
#include "System/Math.hpp"


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


void PhysicsObject::setDirection( const sf::Vector2f& newDirection )
{
	const auto& angle = angleBetweenVectors(-direction_, -normalizeVector(newDirection)) * 180 / PI;
	shape_->rotate(angle);
	direction_ = normalizeVector(newDirection);
}

bool PhysicsObject::intersects(const PhysicsObject& other) const
{
	return shape_->getGlobalBounds().intersects(other.shape_->getGlobalBounds());
}

PlayerPhysicsObject::PlayerPhysicsObject( Physics* physics, GameLogicObject* logicObject) :
	PhysicsObject(physics, logicObject, {physics->getBounds().x/2, physics->getBounds().y/2}, PY::KEEP)
{
	shape_ = std::make_shared<sf::CircleShape>(10.f, 3);
	shape_->rotate(180);
	shape_->setOrigin(shape_->getLocalBounds().width/2, shape_->getLocalBounds().height/2);
	shape_->setFillColor(sf::Color::Red);
}


namespace
{
std::shared_ptr<sf::ConvexShape> makeRandomConvexShape()
{
	constexpr float RADIUS = 20.f;
	std::random_device r;
	std::default_random_engine generator{r()};
	const std::uniform_int_distribution<int> numOfVertixsDistribution(16, 32);
	const std::uniform_real_distribution<float> coordinatesDistribution(.0f, 5.0f);

	auto shape = std::make_shared<sf::ConvexShape>();

	const int& numOfPoints = numOfVertixsDistribution(generator);
	const float& PI_STEP = PI * 2 / numOfPoints;
	shape->setPointCount(numOfPoints);

	for (int index = 0; index < numOfPoints; ++index)
	{
		const auto& x = coordinatesDistribution(generator)+RADIUS*cos(PI_STEP*index);
		const auto& y = coordinatesDistribution(generator)+RADIUS*sin(PI_STEP*index);

		shape->setPoint(index, {x, y});
	}

	return shape;
}
}

AsteroidPhysicsObject::AsteroidPhysicsObject( Physics* physics, GameLogicObject* logicObject, const sf::Vector2f& position ) :
	PhysicsObject(physics, logicObject, position, PY::BOUNCE)
{
	shape_ = makeRandomConvexShape();
	shape_->setOrigin(shape_->getLocalBounds().width/2, shape_->getLocalBounds().height/2);
	shape_->setOutlineColor(sf::Color::White);
	shape_->setOutlineThickness(3.f);
	shape_->setFillColor(sf::Color::Black);
}

ProjectilePhysicsObject::ProjectilePhysicsObject( Physics* physics, GameLogicObject* logicObject,const sf::Vector2f& position ) :
	PhysicsObject(physics, logicObject, position, PY::DESTROY)
{
	shape_ = std::make_shared<sf::CircleShape>(2.5f, 32);
	shape_->setFillColor(sf::Color::Yellow);
}
