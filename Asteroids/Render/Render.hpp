#pragma once

#include <iostream>

#include "SFML/Graphics/Font.hpp"
#include <SFML/Graphics/RenderWindow.hpp>


class BaseRender
{
public:
	BaseRender(sf::RenderWindow* window) : window_(window) {}

	virtual void update() = 0;
protected:
	sf::RenderWindow* window_;
};


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
	~MenuRender() {
		std::cout << "Bingo!";
	}
	void update() override;

	MenuItem handleMouseButton();

private:
	std::vector<sf::Text> menuItems_;
	sf::Font font_;
};



class GameLogic;

class GameplayRender : public BaseRender
{
public:
	GameplayRender( GameLogic* gameLogic, sf::RenderWindow* window) : BaseRender(window), gameLogic_(gameLogic) {}

	void update() override;

private:
	GameLogic* gameLogic_;
};