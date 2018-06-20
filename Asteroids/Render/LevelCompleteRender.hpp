#pragma once

#include "SFML/Graphics/Text.hpp"

#include "Render.hpp"


class LevelCompleteRender : public BaseRender
{
public:
	LevelCompleteRender(sf::RenderWindow* window);
	void update() override;

private:
	sf::Text label_;
};