#include <cassert>
#include <random>

#include "GameLogic/GameLogic.hpp"

#include "Physics.hpp"


namespace
{
	constexpr float PI = 3.14159265358979323846f;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> angleDistribution(-45, 45);
	auto getAngle = [](){ return angleDistribution(generator) * PI / 180.f; };

	sf::Vector2f normalizeVector(const sf::Vector2f& vector)
	{
		const float length = sqrt(vector.x*vector.x + vector.y*vector.y);
		return {vector.x/length, vector.y/length};
	}


	enum Side
	{
		UP,
		RIGHT,
		DOWN,
		LEFT
	};
}

sf::Vector2f Physics::generateRandomVelocity( const sf::Vector2f& position, float speed )
{
	// velocity is chosen in 90 degree arc towards center
	const float angle = getAngle();
	const auto directionToCenter = normalizeVector({-position.x, -position.y});
	return sf::Vector2f(cos(angle)*directionToCenter.x - sin(angle)*directionToCenter.y, sin(angle)*directionToCenter.x + cos(angle)*directionToCenter.y ) * speed;
}

sf::Vector2f Physics::generateRandomPositionOutsideBounds() const
{
	constexpr static float OFFSET = 20.f;
	static std::uniform_int_distribution<int> sideChoiceDistr(0, 3);
	static std::uniform_real<float> xDistribution(0, width_);
	static std::uniform_real<float> yDistribution(0, height_);
	int side = sideChoiceDistr(generator);

	switch (side)
	{
	case UP:
		return {xDistribution(generator), height_ - OFFSET};
	case DOWN:
		return {xDistribution(generator), OFFSET};
	case RIGHT:
		return {width_ - OFFSET, yDistribution(generator)};
	case LEFT:
		return {OFFSET, yDistribution(generator)};
	default:
		assert(false);
		return {};
	}
}

PhysicsObjectPtr Physics::createPhysicsBody(GameLogicObject* obj, const sf::Vector2f& position)
{
	const auto type = obj->getType();
	const auto strategy = LeaveLogicTypeToStrategy[obj->getType()];

	PhysicsObjectPtr ptr = nullptr;

	switch (type)
	{
	case GL::ASTEROID:
		ptr = std::make_shared<AsteroidPhysicsObject>( this, obj, position );
		break;

	case GL::PLAYER:
		ptr = std::make_shared<PlayerPhysicsObject>(this, obj);
		break;

	default: break;
	}
	
	bodies_.insert(ptr);
	return ptr;
}

void Physics::destroyPhysicsBody( const PhysicsObjectPtr body )
{
	bodiesToDestroy_.push_back(body);
}

int Physics::update(float dt)
{
	updatePositions(dt);
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
	std::vector<PhysicsObjectPtr> bodies(bodies_.begin(), bodies_.end());
	const auto s = bodies.size();

	for (size_t i = 0; i < s-1; ++i)
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

void Physics::updatePositions(float dt)
{
	for (const auto& body : bodies_)
		body->move(dt);
}

