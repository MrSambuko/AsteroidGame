#pragma once

#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Text.hpp"

#include "Render/Render.hpp"


namespace sf 
{
	class RenderWindow;
}

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
	sf::Font font_;
};
