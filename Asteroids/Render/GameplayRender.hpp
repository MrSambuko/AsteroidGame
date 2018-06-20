#pragma once

#include <unordered_map>

#include "SFML/Graphics/Text.hpp"

#include "Render.hpp"



class GameLogic;

enum UiElements
{
	LEVEL_LABEL,
	LEVEL_VALUE,
	SCORE_LABEL,
	SCORE_VALUE,
	MAX_SCORE_LABEL,
	MAX_SCORE_VALUE,
	TIME_LABEL,
	TIME_VALUE
};

class GameplayRender : public BaseRender
{
public:
	GameplayRender( GameLogic* gameLogic, sf::RenderWindow* window );

	void update() override;

	void setLevel(int level);
	void setMaxScore(int maxScore);

private:
	void drawShapes() const;
	void drawUi() const;

private:
	std::unordered_map<UiElements, sf::Text> uiElements_;
	GameLogic* gameLogic_;
};
