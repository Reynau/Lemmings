#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	bLeftMouse = bRightMouse = false;
	glClearColor(0.f, 0.f, 0.f, 1.0f);

	cursor = new Cursor();

	mainMenu.init(cursor);
	scene.init(cursor);
	credits.init(cursor);
}

bool Game::update(int deltaTime)
{
	int transition = 0;
	switch (gameState) {
	case GameState::GAME:
		transition = scene.update(deltaTime);
		if (transition == 7) {
			gameState = GameState::DATA;
		}
		break;
	case GameState::CREDITS:
		transition = credits.update(deltaTime);
		if (transition == 4) {
			gameState = GameState::MENU;
		}
		break;
	case GameState::DATA:
		//transition = data.update(deltaTime);
		if (transition == 6) {
			gameState = GameState::GAME;
		}
		if (transition == 8) {
			gameState = GameState::MENU;
		}
		break;
	case GameState::INSTRUCTIONS:
		//transition = instructions.update(deltaTime);
		if (transition == 1) {
			gameState = GameState::MENU;
		}
		break;

	case GameState::MENU:
		transition = mainMenu.update(deltaTime);
		if (transition == 2) {
			gameState = GameState::INSTRUCTIONS;
		}
		if (transition == 5) {
			gameState = GameState::GAME;
		}
		if (transition == 3) {
			gameState = GameState::CREDITS;
		}
		if (transition == 9) {
			// EXIT
		}
		break;
	}
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (gameState) {
	case GameState::GAME:
		scene.render();
		break;
	case GameState::CREDITS:
		credits.render();
		break;
	case GameState::DATA:
		//data.render();
		break;
	case GameState::INSTRUCTIONS:
		//instructions.render();
		break;

	case GameState::MENU:
		mainMenu.render();
		break;
	}
}

void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	if (key == 68 || key == 100) // D
		scene.moveMap(true);
	else if (key == 65 || key == 97) // A
		scene.moveMap(false);
	if (key == 87 || key == 119) // W
		scene.releaseRateUp();
	else if (key == 83 || key == 115) // S
		scene.releaseRateDown();
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
	mouseX = x;
	mouseY = y;
	//mainMenu.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
	//scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
}

void Game::mousePress(int button)
{
	if(button == GLUT_LEFT_BUTTON)
	{
		bLeftMouse = true;
		//mainMenu.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		//scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
	}
	else if(button == GLUT_RIGHT_BUTTON)
	{
		bRightMouse = true;
		//scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
	}
}

void Game::mouseRelease(int button)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		bLeftMouse = false;
		//mainMenu.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
		//scene.mouseMoved(mouseX, mouseY, bLeftMouse, bRightMouse);
	}
	else if(button == GLUT_RIGHT_BUTTON)
		bRightMouse = false;
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





