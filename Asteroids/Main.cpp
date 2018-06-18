// Tutorial from https://www.sfml-dev.org/tutorials/2.4/start-vc.php

#include "SFML/Graphics.hpp"

#include "GameLogic\Scenario.hpp"
#include "GameLogic\GameLogic.hpp"
#include "Physics\Physics.hpp"


constexpr unsigned WIDTH = 1024;
constexpr unsigned HEIGHT = 780;

int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");

	Scenario s("scenarios.ini");

	Physics physics(static_cast<float>(WIDTH), static_cast<float>(HEIGHT));
	GameLogic logic(s, &physics);	
	logic.init();

	logic.createGameObject({ 5.f, .0f }, ASTEROID);
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		logic.update();
		physics.update();
	}

	return 0;
}