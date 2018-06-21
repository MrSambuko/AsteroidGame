# Asteroid Game

## Brief description of how it works:
- Game is handled by Game class. It works as state machine to transition between stages: MENU - GAMEPLAY - LEVEL_COMPLETE - LEVEL_FAILED
- GameLogic class are responsible for handling different game situation - collision, creating new entities, etc
- GameLogicObjects are logical entities that are created by GameLogic. They are responsible for logical behaviour of entities
- Physics is responsible for updating all entities (move, rotate, detect collision)
- PhysicsObjects are physical entities that are created by Physics.
- Render classes are used in different stages of game to draw either text or gameplay
- System are some helper classes

## Known issues:
- no sound. Somehow SFML can't open any of my files. Tried to follow guidelines but still no success :(
- some weired asteroid splitting. Sometimes it works fine (direction is to the side of original big asteroid direction), sometimes both small asteroids fly in same direction
- physics is very basic (using bounding boxes of an objects). Sometimes it leads to wrong collision detection (visibly items don't intersect while logic tells they do).
- some UI tweaks are needed (for example not flying "behind" score)
