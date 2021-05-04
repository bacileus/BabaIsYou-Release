#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "SpriteMap.h"
#include "Physics.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	
	void init(SpriteMap* spritemap, Physics* p);
	void update(int deltaTime);
	void render();
	void setSprite(Sprite* s);

	bool hasLost() const;
	
private:
	glm::ivec2 posPlayer, posBomb;
	int jumpAngle, startY;
	Texture spritesheet;
	Sprite* sprite;
	Physics *physics;
	SpriteMap *spritemap;
	bool isMoving = false;

	bool lost = false;
	bool isPlanting = false;

	float timer = 0.f;
};


#endif // _PLAYER_INCLUDE

