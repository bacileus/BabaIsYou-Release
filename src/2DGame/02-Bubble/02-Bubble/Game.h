#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include "Scene.h"
#include <irrKlang.h>
#include <iostream>
#include <stdio.h>

// include console I/O methods (conio.h for windows, our wrapper in linux)
#include <conio.h>

//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

using namespace irrklang;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

// Game is a singleton (a class with a single instance) that represents our whole application

class Game
{

public:
	Game() {}


	static Game& instance()
	{
		static Game G;

		return G;
	}

	void init();
	bool update(int deltaTime);
	void render();

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

private:
	bool bPlay;                       // Continue to play game?

	/*
	Escena 0: menu principal
	Escena 1-5: niveles
	Escena 6: victory royale
	Escena 7: salir
	Escena 8: lvl selector
	Escena 9: instrucciones
	Escena 10: creditos
	*/
	Scene scene;                      // Scene to render
	ISound* bso_general;

	int currentScene = 0;

	bool keys[256], specialKeys[256]; // Store key states so that 
									  // we can have access at any time

};

#endif // _GAME_INCLUDE