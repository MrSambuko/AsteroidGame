#pragma once

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

#include "Render/Render.hpp"

enum MenuItem
{
	TITLE,
	START,
	EXIT
};

class MenuRender : public BaseRender
{
public:
	MenuRender(sf::RenderWindow* window);
	void update() override;

	MenuItem handleMouseButton();

private:
	std::vector<sf::Text> menuItems_;
};
