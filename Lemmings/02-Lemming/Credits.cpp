#include <iostream>
#include <cmath>
#include <algorithm>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Credits.h"



Credits::Credits()
{
	//map = NULL;
}


Credits::~Credits()
{
	//if (map != NULL)
	//	delete map;
}

void Credits::init(Cursor *cursor)
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

	//quad = Quad::createQuad(0.f, 0.f, 128.f, 128.f, program);

	titleTexture.loadFromFile("images/Lemmings-Logo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	titleTexture.setMinFilter(GL_NEAREST);
	titleTexture.setMagFilter(GL_NEAREST);

	lemmingTexture.loadFromFile("images/lemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
	lemmingTexture.setMinFilter(GL_NEAREST);
	lemmingTexture.setMagFilter(GL_NEAREST);


	sprite = Sprite::createSprite(glm::ivec2(275, 102), glm::vec2(1.f, 1.f), &titleTexture, &simpleTexProgram);
	sprite->setPosition(glm::vec2(20.f, 3.f));

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT + 40), 0.f);
	currentTime = 0.0f;

	this->cursor = cursor;
	this->cursor->initCursor(simpleTexProgram, &lemmingTexture);

	if (!text.init("fonts/Pixellari.ttf"))
		cout << "Could not load font!!!" << endl;

	buttonsStarted = false;
	clicked = false;
}

int Credits::update(int deltaTime)
{
	currentTime += deltaTime;

	if (currentTime < 2000) {
		wid = float(glutGet(GLUT_WINDOW_WIDTH));
		hei = float(glutGet(GLUT_WINDOW_HEIGHT));
		xOffset = roundf(wid * 200.f / 1920.f);
		yOffset = roundf(hei * 650.f / 1080.f);
		sizeFont = roundf(hei * 90.f / 1080.f);
		spaceBut = roundf(hei * 120.f / 1080.f);
	}

	return transition;
}

void Credits::render()
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

	text.render("DEVELOPED BY: ", glm::vec2(xOffset - 3, yOffsetAux - 3), sizeFont, glm::vec4(0.74, 0.89, 0.38, 1));
	text.render("DEVELOPED BY: ", glm::vec2(xOffset + 3, yOffsetAux + 3), sizeFont, glm::vec4(0.51, 0.61, 0.26, 1));
	text.render("DEVELOPED BY: ", glm::vec2(xOffset, yOffsetAux), sizeFont, glm::vec4(0.63, 0.76, 0.32, 1));

	yOffsetAux += spaceBut;

	text.render("JOEL BORRAS ESCODA", glm::vec2(xOffset - 3, yOffsetAux - 3), sizeFont, glm::vec4(0.74, 0.89, 0.38, 1));
	text.render("JOEL BORRAS ESCODA", glm::vec2(xOffset + 3, yOffsetAux + 3), sizeFont, glm::vec4(0.51, 0.61, 0.26, 1));
	text.render("JOEL BORRAS ESCODA", glm::vec2(xOffset, yOffsetAux), sizeFont, glm::vec4(0.63, 0.76, 0.32, 1));

	yOffsetAux += spaceBut;

	text.render("XAVIER LOPEZ REYNAU", glm::vec2(xOffset - 3, yOffsetAux - 3), sizeFont, glm::vec4(0.74, 0.89, 0.38, 1));
	text.render("XAVIER LOPEZ REYNAU", glm::vec2(xOffset + 3, yOffsetAux + 3), sizeFont, glm::vec4(0.51, 0.61, 0.26, 1));
	text.render("XAVIER LOPEZ REYNAU", glm::vec2(xOffset, yOffsetAux), sizeFont, glm::vec4(0.63, 0.76, 0.32, 1));

	yOffsetAux += spaceBut;

	text.render("VJ - 2017 / 2018", glm::vec2(xOffset - 3, yOffsetAux - 3), sizeFont, glm::vec4(0.74, 0.89, 0.38, 1));
	text.render("VJ - 2017 / 2018", glm::vec2(xOffset + 3, yOffsetAux + 3), sizeFont, glm::vec4(0.51, 0.61, 0.26, 1));
	text.render("VJ - 2017 / 2018", glm::vec2(xOffset, yOffsetAux), sizeFont, glm::vec4(0.63, 0.76, 0.32, 1));

	int xOffsetAux = xOffset;
	xOffsetAux += spaceBut * 7;

	text.render("Press left click to go back", glm::vec2(xOffsetAux - 3, yOffsetAux - 3), sizeFont / 2, glm::vec4(0.74, 0.89, 0.38, 1));
	text.render("Press left click to go back", glm::vec2(xOffsetAux + 3, yOffsetAux + 3), sizeFont / 2, glm::vec4(0.51, 0.61, 0.26, 1));
	text.render("Press left click to go back", glm::vec2(xOffsetAux, yOffsetAux), sizeFont / 2, glm::vec4(0.63, 0.76, 0.32, 1));

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	cursor->render();
}

void Credits::resetTransition()
{
	transition = 0;
}

void Credits::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	if (!bLeftButton) clicked = false;
	if (!clicked) {
		if (bLeftButton) {
			transition = 4;
			clicked = true;
		}
		if (bRightButton) {

		}
	}
	cursor->setPos(mouseX, mouseY);
}

void Credits::initShaders()
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