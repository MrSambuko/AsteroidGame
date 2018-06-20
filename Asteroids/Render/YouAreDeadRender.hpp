#pragma once



#include "SFML/Graphics/Text.hpp"

#include "Render.hpp"


class YouAreDeadRender : public BaseRender
{
public:
	YouAreDeadRender(sf::RenderWindow* window);
	void update() override;

private:
	sf::Text label_;
};