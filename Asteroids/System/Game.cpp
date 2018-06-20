#include <cassert>

#include "SFML/Window/Event.hpp"

#include "Render/GameplayRender.hpp"
#include "Render/MenuRender.hpp"
#include "Render/LevelCompleteRender.hpp"
#include "Render/YouAreDeadRender.hpp"

#include "Game.hpp"


Game::~Game()
{
	render_ = nullptr;
	gameLogic_ = nullptr;
	physics_ = nullptr;
}

void Game::update() 
{
	switch (gameState_)
	{
	case GameNamespace::GameState::MENU:
		updateMenuLogic();
		break;

	case GameNamespace::GameState::LEVEL_COMPLETE:
		updateLevelCompleteLogic();
		break;

	case GameNamespace::GameState::GAMEPLAY:
		updateGameplayLogic();
		break;

	case GameNamespace::GameState::YOUR_ARE_DEAD:
		updateYouAreDeadLogic();
		break;

	default:
		assert(false);
		break;
	}
}

void Game::updateEvent(const sf::Event& event)
{
	switch (gameState_)
	{
	case GameNamespace::GameState::MENU:
		updateMenuEvent(event);
		break;

	case GameNamespace::GameState::LEVEL_COMPLETE:
		updateLevelCompleteEvent(event);
		break;

	case GameNamespace::GameState::GAMEPLAY:
		updateGameplayEvent(event);
		break;

	case GameNamespace::GameState::YOUR_ARE_DEAD:
		updateYouAreDeadEvent(event);
		break;

	default:
		assert(false);
		break;
	}
}

void Game::switchState(GameNamespace::GameState newState )
{
	gameState_ = newState;
	switch (newState)
	{
	case GameNamespace::GameState::MENU:
		prepareMenu();
		break;

	case GameNamespace::GameState::GAMEPLAY:
		prepareGameplay();
		break;

	case GameNamespace::GameState::LEVEL_COMPLETE:
		prepareLevelCompleteScreen();
		break;

	case GameNamespace::GameState::YOUR_ARE_DEAD:
		prepareYouAreDeadScreen();
		break;

	default:
		assert(false);
		break;
	}
}

void Game::prepareMenu()
{
	render_ = std::make_shared< MenuRender >(window_);
	gameLogic_ = nullptr;
	physics_ = nullptr;
}

void Game::prepareLevelCompleteScreen()
{
	render_ = std::make_shared<LevelCompleteRender>(window_);
	gameLogic_ = nullptr;
	physics_ = nullptr;
	++currentLevel_;
}


void Game::prepareGameplay()
{
	Scenario scenario("resources/scenarios.ini");
	physics_ = std::make_shared<Physics>(static_cast<float>(window_->getSize().x), static_cast<float>(window_->getSize().y));
	gameLogic_ = std::make_shared<GameLogic>(window_, std::move(scenario), physics_.get());
	render_ = std::make_shared<GameplayRender>(gameLogic_.get(), window_);
	gameLogic_->init(currentLevel_);
	const auto render = dynamic_cast<GameplayRender*>(render_.get());
	render->setLevel(currentLevel_+1);
	render->setMaxScore(scenario.getScenarioDetails(currentLevel_).targetScore);

	start_ = std::chrono::steady_clock::now();
}

void Game::prepareYouAreDeadScreen()
{
	render_ = std::make_shared<YouAreDeadRender>(window_);
	gameLogic_ = nullptr;
	physics_ = nullptr;
	currentLevel_ = 0;
}

void Game::updateMenuEvent( const sf::Event& event )
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		const auto& result = dynamic_cast<MenuRender*>(render_.get())->handleMouseButton();

		switch (result)
		{
		case START:
			gameState_ = GameNamespace::GameState::GAMEPLAY;
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

void Game::updateLevelCompleteEvent(const sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed ||
		event.type == sf::Event::KeyPressed)
	{
		++currentLevel_;
		switchState(GameNamespace::GAMEPLAY);
	}
}

void Game::updateYouAreDeadEvent(const sf::Event & event)
{
	if (event.type == sf::Event::MouseButtonPressed ||
		event.type == sf::Event::KeyPressed)
	{
		switchState(GameNamespace::MENU);
	}
}

void Game::updateMenuLogic() const
{
	render_->update();
}

void Game::updateGameplayLogic()
{
	const auto& diffTime = std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::steady_clock::now() - start_);

	const auto& gameLogicResult = gameLogic_->update(diffTime.count());
	
	if (gameLogicResult == GameLogicState::LEVEL_COMPLETE)
	{
		switchState(GameNamespace::GameState::LEVEL_COMPLETE);
		return;
	}
	if (gameLogicResult == GameLogicState::GAME_OVER)
	{
		switchState(GameNamespace::GameState::YOUR_ARE_DEAD);
		return;
	}
	physics_->update(diffTime.count());
	render_->update();

	start_ = std::chrono::steady_clock::now();
}

void Game::updateLevelCompleteLogic() const
{
	render_->update();
}

void Game::updateYouAreDeadLogic() const
{
	render_->update();
}
