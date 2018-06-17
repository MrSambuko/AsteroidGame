// Tutorial from https://www.sfml-dev.org/tutorials/2.4/start-vc.php

#include <SFML/Graphics.hpp>

#include "GameLogic\GameLogic.hpp"
#include "Physics\Physics.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(1024, 780), "SFML works!");

	Physics physics;
	GameLogic logic(&physics);	
	logic.init();

	logic.createGameObject({ 5.f, .0f });
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