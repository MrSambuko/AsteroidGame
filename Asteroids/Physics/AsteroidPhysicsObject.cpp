
#include <SFML/Graphics/Color.hpp>

#include "AsteroidPhysicsObject.hpp"
#include "System/Common.hpp"
#include "System/Math.hpp"
#include <SFML/Graphics/ConvexShape.hpp>

namespace
{
constexpr const float ASTEROID_SIZE = 20.f;
constexpr const float SMALL_ASTEROID_SIZE = 10.f;

std::shared_ptr<sf::ConvexShape> makeRandomConvexShape(float radius)
{	auto shape = std::make_shared<sf::ConvexShape>();

	const int& numOfPoints = generateRandomInt(16, 32);
	const float& PI_STEP = PI * 2 / numOfPoints;
	shape->setPointCount(numOfPoints);

	for (int index = 0; index < numOfPoints; ++index)
	{
		const auto& x = generateRandomFloat(.0f, 5.0f)+radius*cos(PI_STEP*index);
		const auto& y = generateRandomFloat(.0f, 5.0f)+radius*sin(PI_STEP*index);

		shape->setPoint(index, {x, y});
	}

	return shape;
}
}

AsteroidPhysicsObject::AsteroidPhysicsObject( Physics* physics, GameLogicObject* logicObject, const sf::Vector2f& position ) :
	PhysicsObject(physics, logicObject, position, PY::BOUNCE)
{
	shape_ = makeRandomConvexShape(ASTEROID_SIZE);
	shape_->setOrigin(shape_->getLocalBounds().width/2, shape_->getLocalBounds().height/2);
	shape_->setOutlineColor(sf::Color::White);
	shape_->setOutlineThickness(3.f);
	shape_->setFillColor(sf::Color::Black);
}

SmallAsteroidPhysicsObject::SmallAsteroidPhysicsObject( Physics* physics, GameLogicObject* logicObject, const sf::Vector2f& position ) :
	PhysicsObject(physics, logicObject, position, PY::BOUNCE)
{
	shape_ = makeRandomConvexShape(SMALL_ASTEROID_SIZE);
	shape_->setOrigin(shape_->getLocalBounds().width/2, shape_->getLocalBounds().height/2);
	shape_->setOutlineColor(sf::Color::White);
	shape_->setOutlineThickness(3.f);
	shape_->setFillColor(sf::Color::Black);
}
