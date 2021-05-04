#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include <GL\freeglut_std.h>
#include <irrKlang.h>
#include <stdio.h>

// include console I/O methods (conio.h for windows, our wrapper in linux)
#include <conio.h>

//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

using namespace irrklang;

ISoundEngine* sele = createIrrKlangDevice();


#define SCREEN_X 24
#define SCREEN_Y 24

#define INIT_PLAYER_X_TILES 24
#define INIT_PLAYER_Y_TILES 24

int opcionEscogida = 0;


Scene::Scene()
{
	spritemap = NULL;
	player = NULL;
	quad = NULL;
}

Scene::~Scene()
{
	if (player != NULL)
		delete player;
	if (quad != NULL)
		delete quad;
	for (int i = 0; i < 9; i++)
		if (texQuad[i] != NULL)
			delete texQuad[i];
}


void Scene::init(int id)
{
	initShaders();
	sceneId = id;
	nextScene = -1;
	spritemap = SpriteMap::createSpriteMap(selectScene(), glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	physics = Physics::createPhysicsEngine(spritemap, spritemap->getblockSize(), spritemap->getmapSize(), glm::vec2(SCREEN_X, SCREEN_Y));
	physics->scan();
	if (sceneId > 0 && sceneId < 6) {
		player = new Player();
		player->init(spritemap, physics);
	}
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	//Menu
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(128.f, 128.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	quad = Quad::createQuad(0.f, 128.f, 128.f, 128.f, texProgram);
	texCoords[0] = glm::vec2(0.0f, 0.0f); texCoords[1] = glm::vec2(0.70f, 0.70f); //en la foto, que coordenadas coges de esa foto INSTRUCCIONES
	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	texCoords[0] = glm::vec2(0.0f, 0.0f); texCoords[1] = glm::vec2(0.70f, 0.70f); //JUGAR
	texQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	texCoords[0] = glm::vec2(0.0f, 0.0f); texCoords[1] = glm::vec2(0.70f, 0.70f); //CREDITOS
	texQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	texCoords[0] = glm::vec2(0.0f, 0.0f); texCoords[1] = glm::vec2(0.70f, 0.70f); //SALIR
	texQuad[3] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	texCoords[0] = glm::vec2(0.0f, 0.0f); texCoords[1] = glm::vec2(0.83f, 0.83f); //INSTRUCCIONES pantalla
	texQuad[4] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	texCoords[0] = glm::vec2(0.0f, 0.0f); texCoords[1] = glm::vec2(0.70f, 0.70f); //CREDITOS pantalla
	texQuad[5] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	texCoords[0] = glm::vec2(0.0f, 0.0f); texCoords[1] = glm::vec2(0.70f, 0.70f); //Baba is You Letras
	texQuad[6] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	texCoords[0] = glm::vec2(0.0f, 0.0f); texCoords[1] = glm::vec2(0.70f, 0.70f); //Baba is You Letras
	texQuad[7] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	texCoords[0] = glm::vec2(0.0f, 0.0f); texCoords[1] = glm::vec2(0.80f, 0.80f); //Baba is You Letras
	texQuad[8] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	// Load textures
	texs[0].loadFromFile("images/Menu/InstruccionesBABA.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[1].loadFromFile("images/Menu/JugarBABA.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[2].loadFromFile("images/Menu/CreditosBABA.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[3].loadFromFile("images/Menu/SalirBABA.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[4].loadFromFile("images/Menu/INSTRUCCIONES.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[5].loadFromFile("images/Menu/CREDITOS.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[6].loadFromFile("images/Menu/BABAisYOU.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[7].loadFromFile("images/Menu/flecha.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[8].loadFromFile("images/Menu/VICTORY.png", TEXTURE_PIXEL_FORMAT_RGBA);

}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	if (sceneId > 0 && sceneId < 6) {
		player->update(deltaTime);
	}
	spritemap->update(deltaTime);
	if (physics->hasWin()) nextScene = sceneId + 1;
	if (physics->hasLost()) nextScene = sceneId;

	if (sceneId > 0 && sceneId < 6 && player->hasLost()) nextScene = sceneId;

	if (Game::instance().getSpecialKey(GLUT_KEY_DOWN) && !pulsado && sceneId == 0) {

		pulsado = true;

		if (opcionEscogida < 3) opcionEscogida += 1;
	
	}

	if (Game::instance().getSpecialKey(GLUT_KEY_UP) && !pulsado && sceneId == 0) {

		pulsado = true;

		if (opcionEscogida > 0) opcionEscogida -= 1;

	}

	//pulsados
	if (!Game::instance().getSpecialKey(GLUT_KEY_DOWN) && !Game::instance().getSpecialKey(GLUT_KEY_UP)) {

		pulsado = false;

	}

	Sprite *p;

	if (opcionEscogida == 0 && sceneId == 0) {

		p = spritemap->SpriteMap::findWName(glm::vec2(6 * 24, 6 * 24), "key");
		p->Sprite::cambiaColor(glm::vec3(0.8f, 0.8f, 0.4f)); //JUGAR

		p = spritemap->findWName(glm::vec2(6*24, 9 * 24), "key");
		p->Sprite::cambiaColor(glm::vec3(0.05f, 0.05f, 0.05f));
		p = spritemap->findWName(glm::vec2(6 * 24, 12 * 24), "key");
		p->Sprite::cambiaColor(glm::vec3(0.05f, 0.05f, 0.05f));
		p = spritemap->findWName(glm::vec2(6 * 24, 15 * 24), "key");
		p->Sprite::cambiaColor(glm::vec3(0.05f, 0.05f, 0.05f));
	}

	if (opcionEscogida == 1 && sceneId == 0) {


		p = spritemap->SpriteMap::findWName(glm::vec2(6 * 24, 6 * 24), "key");
		p->Sprite::cambiaColor(glm::vec3(0.05f, 0.05f, 0.05f)); 

		p = spritemap->findWName(glm::vec2(6 * 24, 9 * 24), "key");
		p->Sprite::cambiaColor(glm::vec3(0.8f, 0.8f, 0.4f));	//INSTRUCCIONES

		p = spritemap->findWName(glm::vec2(6 * 24, 12 * 24), "key");
		p->Sprite::cambiaColor(glm::vec3(0.05f, 0.05f, 0.05f));
		p = spritemap->findWName(glm::vec2(6 * 24, 15 * 24), "key");
		p->Sprite::cambiaColor(glm::vec3(0.05f, 0.05f, 0.05f));
	}

	if (opcionEscogida == 2 && sceneId == 0) {

		p = spritemap->SpriteMap::findWName(glm::vec2(6 * 24, 6 * 24), "key");
		p->Sprite::cambiaColor(glm::vec3(0.05f, 0.05f, 0.05f));
		p = spritemap->findWName(glm::vec2(6 * 24, 9 * 24), "key");
		p->Sprite::cambiaColor(glm::vec3(0.05f, 0.05f, 0.05f));

		p = spritemap->findWName(glm::vec2(6 * 24, 12 * 24), "key"); //CREDITOS
		p->Sprite::cambiaColor(glm::vec3(0.8f, 0.8f, 0.4f));

		p = spritemap->findWName(glm::vec2(6 * 24, 15 * 24), "key"); 
		p->Sprite::cambiaColor(glm::vec3(0.05f, 0.05f, 0.05f));
	}

	if (opcionEscogida == 3 && sceneId == 0) {

		p = spritemap->SpriteMap::findWName(glm::vec2(6 * 24, 6 * 24), "key");
		p->Sprite::cambiaColor(glm::vec3(0.05f, 0.05f, 0.05f));
		p = spritemap->findWName(glm::vec2(6 * 24, 9 * 24), "key");
		p->Sprite::cambiaColor(glm::vec3(0.05f, 0.05f, 0.05f));
		p = spritemap->findWName(glm::vec2(6 * 24, 12 * 24), "key");
		p->Sprite::cambiaColor(glm::vec3(0.05f, 0.05f, 0.05f));

		p = spritemap->findWName(glm::vec2(6 * 24, 15 * 24), "key"); //SALIR
		p->Sprite::cambiaColor(glm::vec3(0.8f, 0.8f, 0.4f));
	}

	if (Game::instance().getKey(13) && sceneId == 0) { //Cuando le das al ENTER
		switch (opcionEscogida) {
		case 0:
			sele->play2D("sounds/Seleccion.ogg", false, false, false); // seleccion en menu
			nextScene = 1;
			break;
		case 1:
			sele->play2D("sounds/Seleccion.ogg", false, false, false); // seleccion en menu
			nextScene = 9;
			break;
		case 2:
			sele->play2D("sounds/Seleccion.ogg", false, false, false); // seleccion en menu
			nextScene = 10;
			break;
		case 3:
			nextScene = 7;
			break;
		}
	}
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	spritemap->render();
	if (sceneId > 0 && sceneId < 6) {
		player->render();
	}
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

	if (sceneId == 0) { //MENU

		//Instrucciones


		modelview = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 3.0f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(36.f, 58.f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));

		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[0]->render(texs[0]);

		//Jugar

		modelview = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 3.0f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(35.f, 35.f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));

		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[1]->render(texs[1]);

		//Creditos

		modelview = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 3.0f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(33.f, 83.f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));

		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[2]->render(texs[2]);

		//Salir

		modelview = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 3.0f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(33.f, 108.f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));

		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[3]->render(texs[3]);

		//Baba is You

		modelview = glm::scale(glm::mat4(1.0f), glm::vec3(5.0f, 3.0f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(38.f, 18.f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));

		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[6]->render(texs[6]);

	}

	if (sceneId == 6) { //VICTORY

		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(300.f, 210.f, 0.f));
		modelview = glm::scale(glm::mat4(1.0f), glm::vec3(7.0f, 4.0f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(24.f, 36.f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));

		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[8]->render(texs[8]);
	}

	if (sceneId == 9) { //INSTRUCCIONES	

		//Instrucciones

		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(300.f, 210.f, 0.f));
		modelview = glm::scale(glm::mat4(1.0f), glm::vec3(7.0f, 4.0f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(32.f, 37.f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));

		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[4]->render(texs[4]);
	}

	if (sceneId == 10) { //CREDITOS

	//Creditos

		modelview = glm::translate(glm::mat4(1.0f), glm::vec3(300.f, 210.f, 0.f));
		modelview = glm::scale(glm::mat4(1.0f), glm::vec3(7.0f, 4.0f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(15.f, 28.f, 0.f));
		modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));

		texProgram.setUniformMatrix4f("modelview", modelview);
		texQuad[5]->render(texs[5]);
	}

}

int Scene::getNextScene() const
{
	return nextScene;
}

void Scene::setNextScene(int nextScene)
{
	this->nextScene = nextScene;
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

string Scene::selectScene()
{
	string s = "levels/";
	switch (sceneId) {
	case 0:
		s += "mainmenu";
		break;
	case 1:
		s += "level01";
		break;
	case 2:
		s += "level02";
		break;
	case 3:
		s += "level03";
		break;
	case 4:
		s += "level04";
		break;
	case 5:
		s += "level05";
		break;
	case 6:
		s += "gameover";
		break;
	case 7:
		s += "victory";
		break;
	case 8:
		s += "instructions";
		break;
	case 9:
		s += "credits";
		break;
	}
	s += ".txt";

	return s;
}
