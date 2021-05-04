#ifndef _PHYSICS_INCLUDE
#define _PHYSICS_INCLUDE

#include "SpriteMap.h"

class Physics
{
public:
	static Physics* createPhysicsEngine(SpriteMap* spritemap, int blockSize, glm::vec2 mapSize, glm::vec2 minCoords);

	Physics();
	Physics(SpriteMap* spritemap, int blockSize, glm::vec2 mapSize, glm::vec2 minCoords);
	~Physics();

	void scan();
	void apply(string noun, string op, string prop);
	
	bool collisionMoveLeft(const glm::ivec2& pos);
	bool collisionMoveRight(const glm::ivec2& pos);
	bool collisionMoveDown(const glm::ivec2& pos);
	bool collisionMoveUp(const glm::ivec2& posY);

	bool checkPushableLeft(const glm::ivec2& pos) const;
	bool checkPushableRight(const glm::ivec2& pos) const;
	bool checkPushableDown(const glm::ivec2& pos) const;
	bool checkPushableUp(const glm::ivec2& pos) const;

	bool hasWin() const;
	bool hasLost() const;

	void explode(glm::vec2 pos);

private:
	struct cond {
		string noun, op, prop;
	};

	bool win = false;
	bool lost = false;
	SpriteMap* spritemap;
	glm::vec2 mapSize, minCoords;
	int blockSize;
	vector<cond> conds;
};

#endif