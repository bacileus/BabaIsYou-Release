#ifndef _SPRITEMAP_INCLUDE
#define _SPRITEMAP_INCLUDE

#include "Sprite.h"

class SpriteMap
{
public:
	static SpriteMap *createSpriteMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	SpriteMap(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);
	~SpriteMap();

	void render();
	void update(int deltaTime);
	void free();

	glm::ivec2 getmapSize();
	int getblockSize();

	Sprite* findWName(glm::vec2 position, string name) const;
	Sprite* findWTag(glm::vec2 position, string tag) const;
	Sprite* findWInteraction(glm::vec2 position, string interaction) const;
	Sprite* findWBoth(glm::vec2 position, string tag, string interaction) const;

	Sprite* findPlayer() const;

	void createBomb(glm::vec2 pos);
	void destroy(glm::vec2 pos);

private:
	bool loadLevel(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

	Texture texture;
	ShaderProgram shader;
	vector<Sprite*> spritemap;
	glm::ivec2 mapSize, spritesheetSize;
	glm::vec2 spriteTexSize;
	int spriteSize, blockSize;
};

#endif