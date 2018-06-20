#pragma once

#include <chrono>
#include <memory>

#include "SFML/Graphics/RenderWindow.hpp"

#include "GameLogic/GameLogic.hpp"
#include "Render/Render.hpp"
#include "Render/MenuRender.hpp"


enum GameState
{
	MENU,
	GAMEPLAY
};

class Physics;

class Game final
{
public:
	Game(sf::RenderWindow* window) : window_(window), gameState_(MENU) { switchState(MENU); }
	~Game();

	void update();

	void updateEvent(const sf::Event& event);

private:
	void switchState(GameState newState);
	void prepareMenu();
	void prepareGameplay();

	void updateMenuEvent(const sf::Event& event);
	void updateGameplayEvent(const sf::Event& event) const;
	void updateMenuLogic() const;
	void updateGameplayLogic();

private:
	sf::RenderWindow* window_;
	GameState gameState_;

	std::shared_ptr<GameLogic> gameLogic_;
	std::shared_ptr<Physics> physics_;
	std::shared_ptr<BaseRender> render_;

	std::chrono::time_point<std::chrono::steady_clock> start_;
};
