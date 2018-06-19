#pragma once


#include <SFML/Graphics/RenderWindow.hpp>

class GameLogic;

class Render
{
public:
	Render( GameLogic* gameLogic, sf::RenderWindow* window) : gameLogic_(gameLogic), window_(window) {}

	void update();

private:
	GameLogic* gameLogic_;
	sf::RenderWindow* window_;
};