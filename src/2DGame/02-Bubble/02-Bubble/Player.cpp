#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include <irrKlang.h>
#include <iostream>
#include <stdio.h>

// include console I/O methods (conio.h for windows, our wrapper in linux)
#include <conio.h>

//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll


using namespace irrklang;

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

ISoundEngine* moves = createIrrKlangDevice();

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, STAND_DOWN, STAND_UP, MOVE_LEFT, MOVE_RIGHT, MOVE_DOWN, MOVE_UP
};

void Player::init(SpriteMap* spritemap, Physics* p)
{
	physics = p;
	sprite = spritemap->findPlayer();
	this->spritemap = spritemap;
}

void Player::update(int deltaTime)
{
	if (sprite->getInteraction() != "you") {
		Sprite * s = spritemap->findPlayer();
		if (s != nullptr) sprite = s;
		else {
			if(!lost) moves->play2D("sounds/MuerteBaba.ogg", false, false);
			isMoving = true;
			lost = true;
		}
	}

	bool left = Game::instance().getSpecialKey(GLUT_KEY_LEFT);
	bool right = Game::instance().getSpecialKey(GLUT_KEY_RIGHT);
	bool down = Game::instance().getSpecialKey(GLUT_KEY_DOWN);
	bool up = Game::instance().getSpecialKey(GLUT_KEY_UP);
	bool space = Game::instance().getKey(32);

	if (!left && !right && !down && !up) isMoving = false;

	sprite->update(deltaTime);
	posPlayer = sprite->getPosition();
	if (!isMoving)
	{
		if (left)
		{
			moves->play2D("sounds/MovimientoBaba.ogg", false, false); //Movimiento
			isMoving = true;
			if (sprite->getNoun() == "baba" && sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
			posPlayer.x -= 24;
			if (physics->collisionMoveLeft(posPlayer))
			{
				posPlayer.x += 24;
				if(sprite->getNoun() == "baba") sprite->changeAnimation(STAND_LEFT);
			}
		}
		else if (right)
		{
			moves->play2D("sounds/MovimientoBaba.ogg", false, false); //Movimiento
			isMoving = true;
			if (sprite->getNoun() == "baba" && sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
			posPlayer.x += 24;
			if (physics->collisionMoveRight(posPlayer))
			{
				posPlayer.x -= 24;
				if(sprite->getNoun() == "baba") sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else if (down)
		{
			moves->play2D("sounds/MovimientoBaba.ogg", false, false); //Movimiento
			isMoving = true;
			if (sprite->getNoun() == "baba" && sprite->animation() != MOVE_DOWN) sprite->changeAnimation(MOVE_DOWN);
			posPlayer.y += 24;
			if (physics->collisionMoveDown(posPlayer))
			{
				posPlayer.y -= 24;
				if(sprite->getNoun() == "baba") sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else if (up)
		{
			moves->play2D("sounds/MovimientoBaba.ogg", false, false); //Movimiento
			isMoving = true;
			if (sprite->getNoun() == "baba" && sprite->animation() != MOVE_UP) sprite->changeAnimation(MOVE_UP);
			posPlayer.y -= 24;
			if (physics->collisionMoveUp(posPlayer))
			{
				posPlayer.y += 24;
				if(sprite->getNoun() == "baba") sprite->changeAnimation(STAND_RIGHT);
			}
		}
		else if (sprite->getNoun() == "baba")
		{
			if (sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
			else if (sprite->animation() == MOVE_DOWN)
				sprite->changeAnimation(STAND_DOWN);
			else if (sprite->animation() == MOVE_UP)
				sprite->changeAnimation(STAND_UP);
		}

		if (space && !isPlanting) { // space
			isPlanting = true;
			posBomb = posPlayer;
			spritemap->createBomb(posBomb);
			moves->play2D("sounds/PONERbomba.ogg", false, false); //Movimiento
		}

		if (isPlanting) timer += deltaTime;
		if (isPlanting && timer > 3000.f) {
			moves->play2D("sounds/EXPLOSION.ogg", false, false); //Movimiento
			isPlanting = false;
			spritemap->destroy(posBomb);
			physics->explode(posBomb);
			timer = 0.f;
		}
	}

	sprite->setPosition(glm::vec2(float(posPlayer.x), float(posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setSprite(Sprite* s)
{
	sprite = s;
}

bool Player::hasLost() const
{
	return lost;
}
