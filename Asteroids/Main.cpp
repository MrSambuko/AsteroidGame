// Tutorial from https://www.sfml-dev.org/tutorials/2.4/start-vc.php

#include "SFML/Graphics.hpp"

#include "System/Game.hpp"


constexpr unsigned WIDTH = 1024;
constexpr unsigned HEIGHT = 780;



int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Asteroids");

	Game game(&window);
	
	sf::Event event;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			game.updateEvent(event);
		}
		
		window.clear();

		game.update();

		window.display();
	}

	return 0;
}