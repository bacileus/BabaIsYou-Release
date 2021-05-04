#include "Physics.h"

#include <irrKlang.h>
#include <iostream>
#include <stdio.h>

// include console I/O methods (conio.h for windows, our wrapper in linux)
#include <conio.h>

//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll
using namespace irrklang;

ISoundEngine* obje = createIrrKlangDevice();

Physics* Physics::createPhysicsEngine(SpriteMap* spritemap, int blockSize, glm::vec2 mapSize, glm::vec2 minCoords)
{
	Physics* physics = new Physics(spritemap, blockSize, mapSize, minCoords);

	return physics;
}

Physics::Physics()
{
}

Physics::Physics(SpriteMap* spritemap, int blockSize, glm::vec2 mapSize, glm::vec2 minCoords)
{
	this->spritemap = spritemap;
	this->blockSize = blockSize;
	this->mapSize = mapSize;
	this->minCoords = minCoords;
}

Physics::~Physics()
{
	if (spritemap != NULL) {
		spritemap->free();
		delete spritemap;
	}
}

void Physics::scan()
{
	// horizontal
	vector<cond> newconds;
	for (int j = 0; j < mapSize.y; j++) {
		for (int i = 0; i < mapSize.x; i++) {
			glm::vec2 pos = glm::vec2(i * 24, j * 24);
			Sprite* noun = spritemap->findWTag(pos, "noun");
			if (noun != nullptr) {
				Sprite* op = spritemap->findWTag(pos + glm::vec2(24, 0), "op");
				if (op != nullptr) {
					Sprite* prop = spritemap->findWTag(pos + glm::vec2(48, 0), "property");
					Sprite* skull = spritemap->findWName(pos + glm::vec2(48, 0), "skull");
					if (prop != nullptr) {
						// hemos encontrado una cond
						cond c = { noun->getNoun(), op->getNoun(), prop->getNoun() };
						apply(c.noun, c.op, c.prop);
						newconds.push_back(c);
					}
					else if (skull != nullptr) {
						// fire eat skull
						cond c = { noun->getNoun(), op->getNoun(), skull->getNoun() };
						apply(c.noun, c.op, c.prop);
						newconds.push_back(c);
					}
				}
			}
		}
	}

	// vertical
	for (int i = 0; i < mapSize.y; i++) {
		for (int j = 0; j < mapSize.x; j++) {
			glm::vec2 pos = glm::vec2(i * 24, j * 24);
			Sprite* noun = spritemap->findWTag(pos, "noun");
			if (noun != nullptr) {
				Sprite* op = spritemap->findWTag(pos + glm::vec2(0, 24), "op");
				if (op != nullptr) {
					Sprite* prop = spritemap->findWTag(pos + glm::vec2(0, 48), "property");
					if (prop != nullptr) {
						// hemos encontrado una cond
						cond c = { noun->getNoun(), op->getNoun(), prop->getNoun() };
						apply(c.noun, c.op, c.prop);
						newconds.push_back(c);
					}
				}
			}
		}
	}

	// sound
	for (int i = 0; i < newconds.size(); i++) {
		// find
		bool f = false;
		for (int j = 0; j < conds.size(); j++) {
			if (conds[j].noun == newconds[i].noun &&
				conds[j].op == newconds[i].op &&
				conds[j].prop == newconds[i].prop) {
				f = true;
			}
		}
		if (!f && conds.size() > 0) obje->play2D("sounds/CuandoFLAGisWIN.ogg", false, false, false);
	}

	// reset
	for (int i = 0; i < conds.size(); i++) {
		// find
		bool f = false;
		for (int j = 0; j < newconds.size() && !f; j++) {
			if (conds[i].noun == newconds[j].noun &&
				conds[i].op == newconds[j].op &&
				conds[i].prop == newconds[j].prop) {
				f = true;
			}
		}
		if (!f) apply(conds[i].noun, conds[i].op, "none");
	}
	conds = newconds;
}

void Physics::apply(string noun, string op, string prop)
{
	if (noun == "baba" && prop == "win") {
		obje->play2D("sounds/Victoria.ogg", false, false); // Victory Royale
		win = true;
	}
	if (noun == "baba" && prop == "defeat") {
		obje->play2D("sounds/MuerteBaba.ogg", false, false); // Muerte
		lost = true;
	}
	for (int j = 0; j < mapSize.y; j++) {
		for (int i = 0; i < mapSize.x; i++) {
			Sprite* s = spritemap->findWName(glm::vec2(i * 24, j * 24), noun);
			if (s != nullptr && s->getTag() == "entity") {
				s->setInteraction(prop);
			}
		}
	}
}

bool Physics::collisionMoveLeft(const glm::ivec2& pos)
{
	bool b = false;
	if (spritemap->findWInteraction(glm::vec2(pos.x - 24, pos.y) + glm::vec2(24, 0), "stop") != nullptr)
		b = true;
	if (!b && spritemap->findWInteraction(glm::vec2(pos.x - 24, pos.y) + glm::vec2(24, 0), "win") != nullptr) {
		obje->play2D("sounds/Victoria.ogg", false, false); // Victory Royale
		win = true;
	}
	if (!b && spritemap->findWInteraction(glm::vec2(pos.x - 24, pos.y) + glm::vec2(24, 0), "defeat") != nullptr) {
		obje->play2D("sounds/MuerteBaba.ogg", false, false); // Muerte
		lost = true;
	}
	if (!b && spritemap->findWInteraction(glm::vec2(pos.x - 24, pos.y) + glm::vec2(24, 0), "push") != nullptr) {
		b = !checkPushableLeft(pos);
  }
	scan();
	return b;
}

bool Physics::collisionMoveRight(const glm::ivec2& pos)
{
	bool b = false;
	if (spritemap->findWInteraction(glm::vec2(pos.x + 24, pos.y) + glm::vec2(-24, 0), "stop") != nullptr)
		b = true;
	if (!b && spritemap->findWInteraction(glm::vec2(pos.x + 24, pos.y) + glm::vec2(-24, 0), "win") != nullptr) {
		obje->play2D("sounds/Victoria.ogg", false, false); // Victory Royale
		win = true;
	}
	if (!b && spritemap->findWInteraction(glm::vec2(pos.x + 24, pos.y) + glm::vec2(-24, 0), "defeat") != nullptr) {
		obje->play2D("sounds/MuerteBaba.ogg", false, false); // Muerte
		lost = true;
	}
	if (!b && spritemap->findWInteraction(glm::vec2(pos.x + 24, pos.y) + glm::vec2(-24, 0), "push") != nullptr) {
		b = !checkPushableRight(pos);
	}
	scan();
	return b;
}

bool Physics::collisionMoveDown(const glm::ivec2& pos)
{
	bool b = false;
	if (spritemap->findWInteraction(glm::vec2(pos.x, pos.y + 24) + glm::vec2(0, -24), "stop") != nullptr)
		b = true;
	if (!b && spritemap->findWInteraction(glm::vec2(pos.x, pos.y + 24) + glm::vec2(0, -24), "win") != nullptr) {
		obje->play2D("sounds/Victoria.ogg", false, false); // Victory Royale
		win = true;
	}
	if (!b && spritemap->findWInteraction(glm::vec2(pos.x, pos.y + 24) + glm::vec2(0, -24), "defeat") != nullptr) {
		obje->play2D("sounds/MuerteBaba.ogg", false, false); // Muerte
		lost = true;
	}
	if (!b && spritemap->findWInteraction(glm::vec2(pos.x, pos.y + 24) + glm::vec2(0, -24), "push") != nullptr) {
		b = !checkPushableDown(pos);
	}
	scan();
	return b;
}

bool Physics::collisionMoveUp(const glm::ivec2& pos)
{
	bool b = false;
	if (spritemap->findWInteraction(glm::vec2(pos.x, pos.y - 24) + glm::vec2(0, 24), "stop") != nullptr)
		b = true;
	if (!b && spritemap->findWInteraction(glm::vec2(pos.x, pos.y - 24) + glm::vec2(0, 24), "win") != nullptr) {
		obje->play2D("sounds/Victoria.ogg", false, false); // Victory Royale
		win = true;
	}
	if (!b && spritemap->findWInteraction(glm::vec2(pos.x, pos.y - 24) + glm::vec2(0, 24), "defeat") != nullptr) {
		obje->play2D("sounds/MuerteBaba.ogg", false, false); // Muerte
		lost = true;
	}
	if (!b && spritemap->findWInteraction(glm::vec2(pos.x, pos.y - 24) + glm::vec2(0, 24), "push") != nullptr) {
		b = !checkPushableUp(pos);
	}
	scan();
	return b;
}

bool Physics::checkPushableLeft(const glm::ivec2& pos) const
{
	bool b = true;
	if (spritemap->findWInteraction(glm::vec2(pos.x, pos.y), "stop") != nullptr) b = false;
	Sprite* s = spritemap->findWInteraction(glm::vec2(pos.x, pos.y), "push");
	if (b && s != nullptr) {
		b = checkPushableLeft(glm::vec2(pos.x - 24, pos.y));
		if (b) {
			s->setPosition(s->getPosition() + glm::vec2(-24, 0));
			obje->play2D("sounds/MuevesAlgo.ogg", false, false); //Movimiento objetos
		}
	}
	return b;
}

bool Physics::checkPushableRight(const glm::ivec2& pos) const
{
	bool b = true;
	if (spritemap->findWInteraction(glm::vec2(pos.x, pos.y), "stop") != nullptr) b = false;
	Sprite* s = spritemap->findWInteraction(glm::vec2(pos.x, pos.y), "push");
	if (b && s != nullptr) {
		b = checkPushableRight(glm::vec2(pos.x + 24, pos.y));
		if (b) {
			s->setPosition(s->getPosition() + glm::vec2(24, 0));
			obje->play2D("sounds/MuevesAlgo.ogg", false, false); //Movimiento objetos
		}
	}
	return b;
}

bool Physics::checkPushableDown(const glm::ivec2& pos) const
{
	bool b = true;
	if (spritemap->findWInteraction(glm::vec2(pos.x, pos.y), "stop") != nullptr) b = false;
	Sprite* s = spritemap->findWInteraction(glm::vec2(pos.x, pos.y), "push");
	if (b && s != nullptr) {
		b = checkPushableDown(glm::vec2(pos.x, pos.y + 24));
		if (b) {
			s->setPosition(s->getPosition() + glm::vec2(0, 24));
			obje->play2D("sounds/MuevesAlgo.ogg", false, false); //Movimiento objetos
		}
	}
	return b;
}

bool Physics::checkPushableUp(const glm::ivec2& pos) const
{
	bool b = true;
	if (spritemap->findWInteraction(glm::vec2(pos.x, pos.y), "stop") != nullptr) b = false;
	Sprite* s = spritemap->findWInteraction(glm::vec2(pos.x, pos.y), "push");
	if (b && s != nullptr) {
		b = checkPushableUp(glm::vec2(pos.x, pos.y - 24));
		if (b) {
			s->setPosition(s->getPosition() + glm::vec2(0, -24));
			obje->play2D("sounds/MuevesAlgo.ogg", false, false); //Movimiento objetos
		}
	}
	return b;
}

bool Physics::hasWin() const
{
	return win;
}

bool Physics::hasLost() const
{
	return lost;
}

void Physics::explode(glm::vec2 pos)
{
	int index = -1;

	for (int i = 0; i < conds.size() && index == -1; i++) {
		if (conds[i].op == "eat") {
			index = i;
		}
	}

	if (index != -1) {
		Sprite* s = spritemap->findWName(pos + glm::vec2(-24, 0), conds[index].prop);
		if (s != nullptr) spritemap->destroy(s->getPosition());
		s = spritemap->findWName(pos + glm::vec2(24, 0), conds[index].prop);
		if (s != nullptr) spritemap->destroy(s->getPosition());
		s = spritemap->findWName(pos + glm::vec2(0, -24), conds[index].prop);
		if (s != nullptr) spritemap->destroy(s->getPosition());
		s = spritemap->findWName(pos + glm::vec2(0, 24), conds[index].prop);
		if (s != nullptr) spritemap->destroy(s->getPosition());
	}
}
