#include <iostream>
#include <cmath>
#include <algorithm>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"



Menu::Menu()
{
	//map = NULL;
}


Menu::~Menu()
{
	//if (map != NULL)
	//	delete map;
}

void Menu::init()
{
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(128.f, 28.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f) };

	initShaders();
	quad = Quad::createQuad(0.f, 0.f, 128.f, 128.f, program);
	

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT -1), 0.f); //-20.f, float(CAMERA_WIDTH + 20), float(CAMERA_HEIGHT + 40), 0.f);
	currentTime = 0.0f;
	
	if (!text.init("fonts/Pixellari.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Menu::update(int deltaTime)
{
	currentTime += deltaTime;
	
	if (currentTime < 1000) {
		wid = float(glutGet(GLUT_WINDOW_WIDTH));
		hei = float(glutGet(GLUT_WINDOW_HEIGHT));
		sizeFont = roundf(hei * 58.f / 1080.f);
	}
}

void Menu::render()
{
	glm::mat4 modelview;

	program.use();
	program.setUniformMatrix4f("projection", projection);
	program.setUniform4f("color", 0.5f, 0.3f, 0.0f, 1.0f);

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(128.f, 48.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::scale(modelview, glm::vec3(10.f, 10.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	quad->render();

	program.setUniform4f("color", 0.f, 0.f, 0.f, 1.0f);

	text.render(" MAIN MENU ", glm::vec2(int(wid/6) , int(hei / 4)), sizeFont, glm::vec4(0.63, 0.76, 0.32, 1));
}
void Menu::initShaders()
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
}