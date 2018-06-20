#include <string>

#include "SFML/Graphics/RenderWindow.hpp"

#include "GameLogic/GameLogic.hpp"

#include "GameplayRender.hpp"

GameplayRender::GameplayRender( GameLogic* gameLogic, sf::RenderWindow* window ): 
	BaseRender(window),
	gameLogic_(gameLogic)
{
	sf::Text text;
	text.setFont(font_);

	text.setString("Score: ");
	uiElements_[SCORE_LABEL] = text;

	text.setString("0000");
	text.setPosition(text.getPosition().x + text.getGlobalBounds().width, 0);
	uiElements_[SCORE_VALUE] = text;
	uiElements_[SCORE_VALUE].setString("");

	text.setString("Level: ");
	text.setPosition(window_->getSize().x * 0.5f - text.getGlobalBounds().width * 0.5f, 0);
	uiElements_[LEVEL_LABEL] = text;

	text.setString("00");
	text.setPosition(text.getPosition().x + text.getGlobalBounds().width, 0);
	uiElements_[LEVEL_VALUE] = text;
	uiElements_[LEVEL_VALUE].setString("");
}

void GameplayRender::update()
{
	drawShapes();
	
	// update score
	uiElements_[SCORE_VALUE].setString(std::to_string(gameLogic_->getScore()));
	drawUi();
}

void GameplayRender::drawShapes() const
{
	// get shapes
	const auto& logicObjects = gameLogic_->getObjects();

	for (const auto& object : logicObjects)
	{
		window_->draw(*object->getPhysicalObject()->getShape().get());
	}
}

void GameplayRender::drawUi() const
{
	for (const auto& uiElement : uiElements_)
	{
		window_->draw(uiElement.second);
	}
}
