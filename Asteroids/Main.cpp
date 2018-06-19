// Tutorial from https://www.sfml-dev.org/tutorials/2.4/start-vc.php

#include <chrono>

#include "SFML/Graphics.hpp"
#include "SFML\System\Clock.hpp"

#include "GameLogic\Scenario.hpp"
#include "GameLogic\GameLogic.hpp"
#include "Physics\Physics.hpp"
#include "Render/Render.hpp"


constexpr unsigned WIDTH = 1024;
constexpr unsigned HEIGHT = 780;

int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Asteroids");

	Scenario s("scenarios.ini");
	sf::Clock clock;
	Physics physics(static_cast<float>(WIDTH), static_cast<float>(HEIGHT));
	GameLogic logic(std::move(s), &physics);	
	Render render(&logic, &window);
	logic.init();

	auto start = std::chrono::steady_clock::now();
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
				logic.handleKeyPressed(event.key.code);
			else if (event.type == sf::Event::KeyReleased)
				logic.handleKeyReleased(event.key.code);
		}
		
		window.clear();

		const auto& diffTime = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - start);
		logic.update(diffTime.count());
		physics.update(diffTime.count());
		render.update();

		window.display();
		start = std::chrono::steady_clock::now();
	}

	return 0;
}