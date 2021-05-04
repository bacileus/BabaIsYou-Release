#include "SpriteMap.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, STAND_DOWN, STAND_UP, MOVE_LEFT, MOVE_RIGHT, MOVE_DOWN, MOVE_UP
};

SpriteMap *SpriteMap::createSpriteMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program) {
	SpriteMap* map = new SpriteMap(levelFile, minCoords, program);

	return map;
}

SpriteMap::SpriteMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	loadLevel(levelFile, minCoords, program);
}

SpriteMap::~SpriteMap()
{
	if (spritemap.size() > 0) {
		for (int i = 0; i < spritemap.size(); i++) {
			spritemap[i]->free();
			delete spritemap[i];
		}
	}
}

void SpriteMap::render()
{
	for (int i = 0; i < spritemap.size(); ++i) {
		if (spritemap[i]->getPriority() == -1) spritemap[i]->render();
	}
	for (int i = 0; i < spritemap.size(); ++i) {
		if (spritemap[i]->getPriority() != -1) spritemap[i]->render();
	}
}

void SpriteMap::update(int deltaTime)
{
	for (int i = 0; i < spritemap.size(); ++i) {
		spritemap[i]->update(deltaTime);
	}
}

void SpriteMap::free() {
	for (int i = 0; i < spritemap.size(); ++i) {
		spritemap[i]->free();
	}
}

glm::ivec2 SpriteMap::getmapSize()
{
	return mapSize;
}

int SpriteMap::getblockSize()
{
	return blockSize;
}

Sprite* SpriteMap::findWName(glm::vec2 position, string name) const
{
	for (int i = 0; i < spritemap.size(); ++i) {
		Sprite* s = spritemap[i];
		if (s->getPosition() == position && s->getNoun() == name)
			return spritemap[i];
	}
	return nullptr;
}

Sprite* SpriteMap::findWTag(glm::vec2 position, string tag) const
{
	for (int i = 0; i < spritemap.size(); ++i) {
		Sprite* s = spritemap[i];
		if (s->getPosition() == position && s->getTag() == tag) {
			return spritemap[i];
		}
	}
	return nullptr;
}

Sprite* SpriteMap::findWInteraction(glm::vec2 position, string interaction) const
{
	for (int i = 0; i < spritemap.size(); ++i) {
		Sprite* s = spritemap[i];
		if (s->getPosition() == position && s->getInteraction() == interaction) {
			return spritemap[i];
		}
	}
	return nullptr;
}

Sprite* SpriteMap::findWBoth(glm::vec2 position, string tag, string interaction) const
{
	for (int i = 0; i < spritemap.size(); ++i) {
		Sprite* s = spritemap[i];
		if (s->getPosition() == position && s->getTag() == tag && s->getInteraction() == interaction) {
			return spritemap[i];
		}
	}
	return nullptr;
}

Sprite* SpriteMap::findPlayer() const
{
	vector<Sprite*> you;
	for (int i = 0; i < spritemap.size(); ++i) {
		Sprite* s = spritemap[i];
		if (s->getInteraction() == "you") {
			you.push_back(s);
		}
	}
	if (you.size() == 0) return nullptr;
	return you[rand() % you.size()];
}

void SpriteMap::createBomb(glm::vec2 pos)
{
	float x = 1.f / spritesheetSize.x;
	float y = 1.f / spritesheetSize.y;

	Sprite* place = Sprite::createSprite(glm::vec2(24, 24), glm::vec2(x, y), &texture, &shader);

	int temp = 774;
	place->setId(temp);
	place->setPosition(glm::vec2(pos.x, pos.y));
	place->setNumberAnimations(1);
	place->setAnimationSpeed(0, 5);
	place->addKeyframe(0, glm::vec2(float((temp - 1) % 32) / spritesheetSize.x, float((temp - 1) / 32) / spritesheetSize.y));
	place->addKeyframe(0, glm::vec2(float((temp - 1) % 32) / spritesheetSize.x, float((temp - 1 + 32) / 32) / spritesheetSize.y));
	place->addKeyframe(0, glm::vec2(float((temp - 1) % 32) / spritesheetSize.x, float((temp - 1 + 64) / 32) / spritesheetSize.y));
	place->changeAnimation(0);
	place->setAtr();	

	spritemap[pos.y / 24 * 25 + pos.x / 24 - 26] = place;
}

void SpriteMap::destroy(glm::vec2 pos)
{
	float x = 1.f / spritesheetSize.x;
	float y = 1.f / spritesheetSize.y;
	
	Sprite* blank = Sprite::createSprite(glm::vec2(24, 24), glm::vec2(x, y), &texture, &shader);
	blank->setId(0);
	blank->setPosition(glm::vec2(pos.x, pos.y));

	spritemap[pos.y / 24 * 25 + pos.x / 24 - 26] = blank;
}

bool SpriteMap::loadLevel(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program) {
	shader = program;
	
	ifstream fin;
	string line, spritesheetFile;
	stringstream sstream;
	char sprite;

	fin.open(levelFile.c_str());
	if (!fin.is_open()) return false;

	getline(fin, line);
	if (line.compare(0, 9, "SPRITEMAP") != 0) return false;

	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;

	getline(fin, line);
	sstream.str(line);
	sstream >> spriteSize >> blockSize;

	getline(fin, line);
	sstream.str(line);
	sstream >> spritesheetFile;

	texture.loadFromFile(spritesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	texture.setWrapS(GL_CLAMP_TO_EDGE);
	texture.setWrapT(GL_CLAMP_TO_EDGE);
	texture.setMinFilter(GL_NEAREST);
	texture.setMagFilter(GL_NEAREST);

	getline(fin, line);
	sstream.str(line);
	sstream >> spritesheetSize.x >> spritesheetSize.y;
	spriteTexSize = glm::vec2(1.f / spriteTexSize.x, 1.f / spriteTexSize.y);

	float x = 1.f / spritesheetSize.x;
	float y = 1.f / spritesheetSize.y;
	for (int j = 0; j < mapSize.y; j++)
	{
		getline(fin, line);
		sstream.str(line);
		for (int i = 0; i < mapSize.x; i++)
		{
			int temp;
			sstream >> temp;
			Sprite * s = Sprite::createSprite(glm::vec2(24, 24), glm::vec2(x, y), &texture, &program);
			s->setId(temp);
			s->setPosition(glm::vec2(blockSize * i + minCoords.x, blockSize * j + minCoords.y));
			if (temp != 0) {
				if (temp == 4) {
					s->setNumberAnimations(8);

					// idle
					s->setAnimationSpeed(STAND_LEFT, 4);
					s->addKeyframe(STAND_LEFT, glm::vec2(11.f * x, 0.f * y));
					s->addKeyframe(STAND_LEFT, glm::vec2(11.f * x, 1.f * y));
					s->addKeyframe(STAND_LEFT, glm::vec2(11.f * x, 2.f * y));

					s->setAnimationSpeed(STAND_RIGHT, 4);
					s->addKeyframe(STAND_RIGHT, glm::vec2(1.f * x, 0.f * y));
					s->addKeyframe(STAND_RIGHT, glm::vec2(1.f * x, 1.f * y));
					s->addKeyframe(STAND_RIGHT, glm::vec2(1.f * x, 2.f * y));

					s->setAnimationSpeed(STAND_DOWN, 4);
					s->addKeyframe(STAND_DOWN, glm::vec2(16.f * x, 0.f * y));
					s->addKeyframe(STAND_DOWN, glm::vec2(16.f * x, 1.f * y));
					s->addKeyframe(STAND_DOWN, glm::vec2(16.f * x, 2.f * y));

					s->setAnimationSpeed(STAND_UP, 4);
					s->addKeyframe(STAND_UP, glm::vec2(7.f * x, 0.f * y));
					s->addKeyframe(STAND_UP, glm::vec2(7.f * x, 1.f * y));
					s->addKeyframe(STAND_UP, glm::vec2(7.f * x, 2.f * y));

					// movement
					s->setAnimationSpeed(MOVE_LEFT, 2);
					s->addKeyframe(MOVE_LEFT, glm::vec2(12.f * x, 0.f * y));
					s->addKeyframe(MOVE_LEFT, glm::vec2(12.f * x, 1.f * y));
					s->addKeyframe(MOVE_LEFT, glm::vec2(12.f * x, 2.f * y));

					s->setAnimationSpeed(MOVE_RIGHT, 2);
					s->addKeyframe(MOVE_RIGHT, glm::vec2(2.f * x, 0.f * y));
					s->addKeyframe(MOVE_RIGHT, glm::vec2(2.f * x, 1.f * y));
					s->addKeyframe(MOVE_RIGHT, glm::vec2(2.f * x, 2.f * y));

					s->setAnimationSpeed(MOVE_DOWN, 2);
					s->addKeyframe(MOVE_DOWN, glm::vec2(17.f * x, 0.f * y));
					s->addKeyframe(MOVE_DOWN, glm::vec2(17.f * x, 1.f * y));
					s->addKeyframe(MOVE_DOWN, glm::vec2(17.f * x, 2.f * y));

					s->setAnimationSpeed(MOVE_UP, 2);
					s->addKeyframe(MOVE_UP, glm::vec2(8.f * x, 0.f * y));
					s->addKeyframe(MOVE_UP, glm::vec2(8.f * x, 1.f * y));
					s->addKeyframe(MOVE_UP, glm::vec2(8.f * x, 2.f * y));
				}
				else {
					s->setNumberAnimations(1);
					s->setAnimationSpeed(0, 5);
					s->addKeyframe(0, glm::vec2(float((temp - 1) % 32) / spritesheetSize.x, float((temp - 1) / 32) / spritesheetSize.y));
					s->addKeyframe(0, glm::vec2(float((temp - 1) % 32) / spritesheetSize.x, float((temp - 1 + 32) / 32) / spritesheetSize.y));
					s->addKeyframe(0, glm::vec2(float((temp - 1) % 32) / spritesheetSize.x, float((temp - 1 + 64) / 32) / spritesheetSize.y));
				}
				s->setAtr();
				s->changeAnimation(0);			
			}
			spritemap.push_back(s);
		}
	}

	return true;
}
