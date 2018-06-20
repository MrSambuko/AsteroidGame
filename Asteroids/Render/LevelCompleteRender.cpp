#include "SFML/Graphics/RenderWindow.hpp"

#include "LevelCompleteRender.hpp"

LevelCompleteRender::LevelCompleteRender(sf::RenderWindow * window) : BaseRender(window)
{
	label_.setFont(font_);
	label_.setString("Level Complete!\nPress Any Key To Continue");
	label_.setPosition(window_->getSize().x*0.5f - label_.getLocalBounds().width / 2,
					  window_->getSize().y*0.5f - label_.getLocalBounds().height / 2);
	label_.setFillColor(sf::Color::Red);
}

void LevelCompleteRender::update()
{
	window_->draw(label_);
}

