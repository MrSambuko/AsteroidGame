#pragma once

#include "Render.hpp"


class GameLogic;


class GameplayRender : public BaseRender
{
public:
	GameplayRender( GameLogic* gameLogic, sf::RenderWindow* window) : BaseRender(window), gameLogic_(gameLogic) {}

	void update() override;

private:
	GameLogic* gameLogic_;
};
