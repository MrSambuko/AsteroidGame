#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Mouse.hpp"

#include "MenuRender.hpp"




MenuRender::MenuRender(sf::RenderWindow * window) : BaseRender(window)
{
	const float middleX = window_->getSize().x * .5f;
	const auto& setX = [&](const sf::Text& text){return middleX - text.getGlobalBounds().width*.5f;};

	sf::Text text;
	text.setFont(font_);

	text.setString("Asteroids");
	sf::Vector2f position(setX(text), 0);
	text.setPosition(position);
	text.setFillColor(sf::Color::Red);
	const auto& bounds = text.getGlobalBounds();
	menuItems_.push_back(text);

	position = {setX(text), window_->getSize().y * .5f + bounds.height + 10 };
	text.setString("Start");
	text.setPosition(position);
	menuItems_.push_back(text);

	text.setString("Exit");
	position = {setX(text), position.y + bounds.height + 10};
	text.setPosition(position);
	menuItems_.push_back(text);
}

void MenuRender::update()
{
	for (const auto& text : menuItems_)
		window_->draw(text);
}

MenuItem MenuRender::handleMouseButton()
{
	const sf::Vector2f mousePosition(static_cast<float>(sf::Mouse::getPosition(*window_).x), 
									 static_cast<float>(sf::Mouse::getPosition(*window_).y));

	for ( size_t index = 0; index < menuItems_.size(); ++index )
	{
		if (menuItems_[index].getGlobalBounds().contains(mousePosition))
			return static_cast<MenuItem>(index);
	}
	return TITLE;
}
