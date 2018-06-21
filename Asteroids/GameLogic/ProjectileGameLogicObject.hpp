#pragma once

#include "SFML/System/Vector2.hpp"

#include "GameLogicObject.hpp"


class GameLogic;

class ProjectileGameLogicObject : public GameLogicObject
{
public:
	ProjectileGameLogicObject( GameLogic* logic, const sf::Vector2f& position, const sf::Vector2f& direction, float speed );
};
