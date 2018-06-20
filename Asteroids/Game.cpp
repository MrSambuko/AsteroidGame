#include "SFML/Window/Event.hpp"

#include "Render/GameplayRender.hpp"

#include "Game.hpp"


void Game::update() 
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

void Game::updateEvent(const sf::Event& event)
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


void Game::prepareGameplay()
{
	Scenario scenario("scenarios.ini");
	physics_ = std::make_shared<Physics>(static_cast<float>(window_->getSize().x), static_cast<float>(window_->getSize().y));
	gameLogic_ = std::make_shared<GameLogic>(window_, std::move(scenario), physics_.get());
	render_ = std::make_shared<GameplayRender>(gameLogic_.get(), window_);
	gameLogic_->init();

	start_ = std::chrono::steady_clock::now();
}

void Game::updateMenuEvent( const sf::Event& event )
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		const auto& result = dynamic_cast<MenuRender*>(render_.get())->handleMouseButton();

		switch (result)
		{
		case START:
			gameState_ = GAMEPLAY;
			prepareGameplay();
			break;

		case EXIT:
			window_->close();
			break;

		default:
			break;
		}
	}
}


void Game::updateGameplayEvent(const sf::Event& event) const
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

void Game::updateGameplayLogic()
{
	const auto& diffTime = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - start_);

	gameLogic_->update(diffTime.count());
	physics_->update(diffTime.count());
	render_->update();

	start_ = std::chrono::steady_clock::now();
}