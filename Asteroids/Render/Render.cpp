#include "GameLogic/GameLogic.hpp"

#include "Render.hpp"

void Render::update()
{
	// get shapes
	const auto& logicObjects = gameLogic_->getObjects();

	for (const auto& object : logicObjects)
	{
		window_->draw(*object->getPhysicalObject()->getShape().get());
	}
}
