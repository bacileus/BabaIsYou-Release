#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
ISoundEngine* engine = createIrrKlangDevice();

void Game::init()
{
	bPlay = true;
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	scene.init(currentScene);

	if (currentScene == 0 || currentScene == 9) bso_general = engine->play2D("sounds/BSOMenu.ogg", true, false, true); //Soundtrack
	else if (currentScene == 10) bso_general = engine->play2D("sounds/BSOcreditos.ogg", true, false, true); //Soundtrack
	else if (currentScene == 6) bso_general = engine->play2D("sounds/VICTORY.ogg", true, false, true); // Victory
	else bso_general = engine->play2D("sounds/BSOlevels.ogg", true, false, true); //Soundtrack
	engine->setSoundVolume(0.3f);
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	int nextScene = scene.getNextScene();
	if (nextScene == 7) return false;
	else if (nextScene != -1) {
		currentScene = nextScene;
		bso_general->stop();
		bso_general->drop();
		init();
	}
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene.render();
}

void Game::keyPressed(int key)
{
	if (key == 27) {// Escape code
		currentScene = 0;
		bso_general->stop();
		bso_general->drop();
		init();
	}

	if (key == 114) { // r
		bso_general->stop();
		bso_general->drop();
		init();
	}

	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}



