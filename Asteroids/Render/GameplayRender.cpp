#include "SFML/Graphics/RenderWindow.hpp"

#include "GameLogic/GameLogic.hpp"

#include "GameplayRender.hpp"

void GameplayRender::update()
{
	// get shapes
	const auto& logicObjects = gameLogic_->getObjects();

	for (const auto& object : logicObjects)
	{
		window_->draw(*object->getPhysicalObject()->getShape().get());
	}
}
