#pragma once

#include <chrono>
#include <memory>

#include "SFML/Graphics/RenderWindow.hpp"

#include "GameLogic/GameLogic.hpp"
#include "Render/Render.hpp"


namespace GameNamespace 
{
	enum GameState
	{
		MENU,
		GAMEPLAY,
		LEVEL_COMPLETE,
		YOUR_ARE_DEAD
	};
}


class Physics;

class Game final
{
public:
	Game(sf::RenderWindow* window) : window_(window), gameState_(GameNamespace::GameState::MENU) { switchState(GameNamespace::GameState::MENU); }
	~Game();

	void update();

	void updateEvent(const sf::Event& event);

private:
	Game(const Game&&) noexcept {};
	Game(const Game& ) = default;
	Game& operator = (const Game&& ) { return *this; }
	Game& operator = (const Game& ) { return *this; }

	void switchState(GameNamespace::GameState newState);
	void prepareMenu();
	void prepareLevelCompleteScreen();
	void prepareGameplay();
	void prepareYouAreDeadScreen();

	void updateMenuEvent(const sf::Event& event);
	void updateGameplayEvent(const sf::Event& event) const;
	void updateLevelCompleteEvent(const sf::Event& event);
	void updateYouAreDeadEvent(const sf::Event& event);
	void updateMenuLogic() const;	
	void updateGameplayLogic();
	void updateLevelCompleteLogic() const;
	void updateYouAreDeadLogic() const;

private:
	sf::RenderWindow* window_;
	GameNamespace::GameState gameState_;

	std::shared_ptr<GameLogic> gameLogic_;
	std::shared_ptr<Physics> physics_;
	std::shared_ptr<BaseRender> render_;

	int currentLevel_ = 0;

	std::chrono::time_point<std::chrono::steady_clock> start_;
};
