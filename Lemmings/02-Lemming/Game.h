#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Menu.h"
#include "Credits.h"
#include "Results.h"
#include "Instructions.h"

// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
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
	Cursor *cursor;

	bool bPlay;                       // Continue to play game?
	Scene scene;                      // Scene to render
	Menu mainMenu;		
	Credits credits;
	Results results;
	Instructions instructions;

	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time
	int mouseX, mouseY;               // Mouse position
	bool bLeftMouse, bRightMouse;     // Mouse button states

	enum GameState {
		MENU, INSTRUCTIONS, CREDITS, GAME, DATA
	};

	GameState gameState = GameState::MENU;
};


#endif // _GAME_INCLUDE


