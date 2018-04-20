#include <iostream>
#include <cmath>
#include <algorithm>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Instructions.h"



Instructions::Instructions()
{
	//map = NULL;
}


Instructions::~Instructions()
{
	//if (map != NULL)
	//	delete map;
}

void Instructions::init(Cursor *cursor)
{
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(glutGet(GLUT_WINDOW_WIDTH) + 56, glutGet(GLUT_WINDOW_HEIGHT) + 56) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(8.f, 8.f) };

	initShaders();

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile("images/menu_background_old.png", TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile("images/menu_background_mask_old.png", TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	titleTexture.loadFromFile("images/Lemmings-Logo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	titleTexture.setMinFilter(GL_NEAREST);
	titleTexture.setMagFilter(GL_NEAREST);

	lemmingTexture.loadFromFile("images/lemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
	lemmingTexture.setMinFilter(GL_NEAREST);
	lemmingTexture.setMagFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(75, 28), glm::vec2(1.f, 1.f), &titleTexture, &simpleTexProgram);
	sprite->setPosition(glm::vec2(115.f, 3.f));

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT + 40), 0.f);
	currentTime = 0.0f;

	this->cursor = cursor;
	this->cursor->initCursor(simpleTexProgram, &lemmingTexture);

	if (!text.init("fonts/Pixellari.ttf"))
		cout << "Could not load font!!!" << endl;

	clicked = false;
}

int Instructions::update(int deltaTime)
{
	currentTime += deltaTime;

	if (currentTime < 2000) {
		wid = float(glutGet(GLUT_WINDOW_WIDTH));
		hei = float(glutGet(GLUT_WINDOW_HEIGHT));
		xOffset = roundf(wid * 100.f / 1920.f);
		yOffset = roundf(hei * 250.f / 1080.f);
		sizeFont = roundf(hei * 50.f / 1080.f);
		spaceBut = roundf(hei * 120.f / 1080.f);
	}


	return transition;
}

void Instructions::render()
{
	glm::mat4 modelview;

	maskedTexProgram.use();
	maskedTexProgram.setUniformMatrix4f("projection", projection);
	maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	maskedTexProgram.setUniformMatrix4f("modelview", modelview);
	map->render(maskedTexProgram, colorTexture, maskTexture);

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	sprite->render();

	program.use();
	program.setUniformMatrix4f("projection", projection);
	program.setUniform4f("color", 0.f, 0.f, 0.f, 1.0f);

	int yOffsetAux = yOffset;

	text.render("When in game, press key 'W' to increment release rating.", glm::vec2(xOffset - 3, yOffsetAux - 3), sizeFont, glm::vec4(0.41, 0, 0.45, 1));
	text.render("When in game, press key 'W' to increment release rating.", glm::vec2(xOffset + 3, yOffsetAux + 3), sizeFont, glm::vec4(0.21, 0, 0.24, 1));
	text.render("When in game, press key 'W' to increment release rating.", glm::vec2(xOffset, yOffsetAux), sizeFont, glm::vec4(0.32, 0, 0.36, 1));

	yOffsetAux += spaceBut;

	text.render("You can also press key 'S' to decrease release rating.", glm::vec2(xOffset - 3, yOffsetAux - 3), sizeFont, glm::vec4(0.41, 0, 0.45, 1));
	text.render("You can also press key 'S' to decrease release rating.", glm::vec2(xOffset + 3, yOffsetAux + 3), sizeFont, glm::vec4(0.21, 0, 0.24, 1));
	text.render("You can also press key 'S' to decrease release rating.", glm::vec2(xOffset, yOffsetAux), sizeFont, glm::vec4(0.32, 0, 0.36, 1));

	yOffsetAux += spaceBut * 1.5;

	text.render("For camera movement, press key 'A' to go to left.", glm::vec2(xOffset - 3, yOffsetAux - 3), sizeFont, glm::vec4(0.41, 0, 0.45, 1));
	text.render("For camera movement, press key 'A' to go to left.", glm::vec2(xOffset + 3, yOffsetAux + 3), sizeFont, glm::vec4(0.21, 0, 0.24, 1));
	text.render("For camera movement, press key 'A' to go to left.", glm::vec2(xOffset, yOffsetAux), sizeFont, glm::vec4(0.32, 0, 0.36, 1));

	yOffsetAux += spaceBut;

	text.render("For the other direction press key 'D' to go to right.", glm::vec2(xOffset - 3, yOffsetAux - 3), sizeFont, glm::vec4(0.41, 0, 0.45, 1));
	text.render("For the other direction press key 'D' to go to right.", glm::vec2(xOffset + 3, yOffsetAux + 3), sizeFont, glm::vec4(0.21, 0, 0.24, 1));
	text.render("For the other direction press key 'D' to go to right.", glm::vec2(xOffset, yOffsetAux), sizeFont, glm::vec4(0.32, 0, 0.36, 1));

	yOffsetAux += spaceBut * 1.5;

	text.render("PRO tip: if you move your mouse to the edge and also press the key", glm::vec2(xOffset - 3, yOffsetAux - 3), sizeFont, glm::vec4(0.41, 0, 0.45, 1));
	text.render("PRO tip: if you move your mouse to the edge and also press the key", glm::vec2(xOffset + 3, yOffsetAux + 3), sizeFont, glm::vec4(0.21, 0, 0.24, 1));
	text.render("PRO tip: if you move your mouse to the edge and also press the key", glm::vec2(xOffset, yOffsetAux), sizeFont, glm::vec4(0.32, 0, 0.36, 1));

	yOffsetAux += spaceBut;

	text.render(" for camera movement, you can move the camera faster!", glm::vec2(xOffset - 3, yOffsetAux - 3), sizeFont, glm::vec4(0.41, 0, 0.45, 1));
	text.render(" for camera movement, you can move the camera faster!", glm::vec2(xOffset + 3, yOffsetAux + 3), sizeFont, glm::vec4(0.21, 0, 0.24, 1));
	text.render(" for camera movement, you can move the camera faster!", glm::vec2(xOffset, yOffsetAux), sizeFont, glm::vec4(0.32, 0, 0.36, 1));

	int xOffsetAux = xOffset;
	xOffsetAux += spaceBut * 7;
	yOffsetAux += spaceBut / 2;

	text.render("Press right click to go back", glm::vec2(xOffsetAux - 2, yOffsetAux - 2), sizeFont / 1.5, glm::vec4(0.74, 0.89, 0.38, 1));
	text.render("Press right click to go back", glm::vec2(xOffsetAux + 2, yOffsetAux + 2), sizeFont / 1.5, glm::vec4(0.51, 0.61, 0.26, 1));
	text.render("Press right click to go back", glm::vec2(xOffsetAux, yOffsetAux), sizeFont / 1.5, glm::vec4(0.63, 0.76, 0.32, 1));
	

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	cursor->render();
}

void Instructions::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	if (bLeftButton) {

	}
	if (bRightButton) {
		transition = 1;
	}
	cursor->setPos(mouseX, mouseY);
}

void Instructions::initShaders()
{
	Shader vShader, fShader;
	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}

	program.init();
	program.addShader(vShader);
	program.addShader(fShader);
	program.link();
	if (!program.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << program.log() << endl << endl;
	}
	program.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();


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
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if (!simpleTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleTexProgram.log() << endl << endl;
	}
	simpleTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();



	vShader.initFromFile(VERTEX_SHADER, "shaders/maskedTexture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/maskedTexture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	maskedTexProgram.init();
	maskedTexProgram.addShader(vShader);
	maskedTexProgram.addShader(fShader);
	maskedTexProgram.link();
	if (!maskedTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << maskedTexProgram.log() << endl << endl;
	}
	maskedTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Instructions::resetTransition()
{
	transition = 0;
}
