#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "SpriteMap.h"
#include "Player.h"
#include "Quad.h"
#include "TexturedQuad.h"
#include "Physics.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(int id);
	void update(int deltaTime);
	void render();
	int getNextScene() const;
	void setNextScene(int nextScene);

private:
	void initShaders();
	string selectScene();

private:
	Quad* quad;
	Texture texs[9];
	TexturedQuad* texQuad[9];
	int sceneId;
	int nextScene = -1;
	SpriteMap* spritemap;
	Physics* physics;

	Player* player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	bool pulsado = false;

};


#endif // _SCENE_INCLUDE