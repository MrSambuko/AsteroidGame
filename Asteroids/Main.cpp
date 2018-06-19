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

enum GameState
{
	MENU,
	GAMEPLAY
};

class Game final
{
public:
	Game(sf::RenderWindow* window) : window_(window), gameState_(MENU) { prepareMenu(); }

	void update() 
	{
		switch (gameState_)
		{
		case MENU:
			updateMenuLogic();
			break;

		case GAMEPLAY:
			updateGameplayLogic();
			break;
		}
	}

	void updateEvent(const sf::Event& event)
	{
		switch (gameState_)
		{
		case MENU:
			updateMenuEvent(event);
			break;

		case GAMEPLAY:
			updateGameplayEvent(event);
			break;
		}
	}

private:
	void prepareMenu()
	{
		render_ = std::make_shared < MenuRender >(window_);
	}

	void prepareGameplay()
	{
		Scenario scenario("scenarios.ini");
		physics_ = std::make_shared<Physics>(static_cast<float>(WIDTH), static_cast<float>(HEIGHT));
		gameLogic_ = std::make_shared<GameLogic>(window_, std::move(scenario), physics_.get());
		render_ = std::make_shared<GameplayRender>(gameLogic_.get(), window_);
		gameLogic_->init();

		start_ = std::chrono::steady_clock::now();
	}

	void updateMenuEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			auto result = dynamic_cast<MenuRender*>(render_.get())->handleMouseButton();

			switch (result)
			{
			case START:
				gameState_ = GAMEPLAY;
				prepareGameplay();
				break;

			case EXIT:
				window_->close();
			}
		}
	}

	void updateGameplayEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
			gameLogic_->handleKeyPressed(event.key.code);
		else if (event.type == sf::Event::KeyReleased)
			gameLogic_->handleKeyReleased(event.key.code);
		if (event.type == sf::Event::MouseButtonPressed)
			gameLogic_->handleMousePressedEvent(event.mouseButton.button);
		else if (event.type == sf::Event::MouseButtonReleased)
			gameLogic_->handleMouseReleasedEvent(event.mouseButton.button);
	}

	void updateMenuLogic()
	{
		render_->update();
	}

	void updateGameplayLogic()
	{
		const auto& diffTime = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - start_);

		gameLogic_->update(diffTime.count());
		physics_->update(diffTime.count());
		render_->update();

		start_ = std::chrono::steady_clock::now();
	}

private:
	sf::RenderWindow* window_;
	GameState gameState_;

	std::shared_ptr<GameLogic> gameLogic_;
	std::shared_ptr<Physics> physics_;
	std::shared_ptr<BaseRender> render_;



	std::chrono::time_point<std::chrono::steady_clock> start_;
};



int main()
{
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Asteroids");

	Game game(&window);
	
	while (window.isOpen())
	{
		sf::Event event;
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