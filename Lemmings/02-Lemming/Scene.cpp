#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"


Scene::Scene()
{
	map = NULL;
}

Scene::~Scene()
{
	if(map != NULL)
		delete map;
}


void Scene::init()
{
	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT))};
	glm::vec2 texCoords[2] = {glm::vec2(120.f / 512.0, 0.f), glm::vec2((120.f + 320.f) / 512.0f, 160.f / 256.0f)};

	initShaders();
	initLevels();

	Level level = levels[actualLevel];

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile(level.colorTextureFile, TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile(level.maskTextureFile, TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	
	lemming.init(glm::vec2(60, 30), simpleTexProgram);
	lemming.setMapMask(&maskTexture);

	spawnedLemmings = 0;

	for (int i = 0; i < level.lemmingsToSpawn; ++i) {
		Lemming * lem = new Lemming();
		lem->init(level.spawnPosition, simpleTexProgram);
		lem->setMapMask(&maskTexture);

		lemmings.push_back(lem);
	}
}

unsigned int x = 0;

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;

	Level level = levels[actualLevel];
	cout << actualLevel << endl << spawnedLemmings << endl;

	// Spawn lemmings every second
	int sec = int(currentTime / 1000);

	//if (sec == 15) changeLevel(1);

	if (spawnedLemmings < level.lemmingsToSpawn && spawnedLemmings <= sec) {
		++spawnedLemmings;
	}

	for (int i = 0; i < spawnedLemmings; ++i) {
		lemmings[i]->update(deltaTime);
	}


	lemming.update(deltaTime);
}

void Scene::render()
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

	for (int i = 0; i < spawnedLemmings; ++i) {
		lemmings[i]->render();
	}


	lemming.render();
}

void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	if(bLeftButton)
		eraseMask(mouseX, mouseY);
	else if(bRightButton)
		applyMask(mouseX, mouseY);
}

void Scene::eraseMask(int mouseX, int mouseY)
{
	int posX, posY;

	posX = mouseX / 3 + 120;
	posY = mouseY / 3;
	int r = 10;
	int dx;
	int dy;
	for (int y = max(0, posY - r); y <= min(maskTexture.height() - 1, posY + r); y++) {
		for (int x = max(0, posX - r); x <= min(maskTexture.width() - 1, posX + r); x++) {
			dx = posX - x; // horizontal offset
			dy = posY - y; // vertical offset
			if ((dx*dx + dy * dy) <= (r*r))
			{
				maskTexture.setPixel(x, y, 0);
			}
		}
	}
}

void Scene::applyMask(int mouseX, int mouseY)
{
	int posX, posY;

	posX = mouseX / 3 + 120;
	posY = mouseY / 3;
	int r = 10;
	int dx;
	int dy;
	for (int y = max(0, posY - r); y <= min(maskTexture.height() - 1, posY + r); y++) {
		for (int x = max(0, posX - r); x <= min(maskTexture.width() - 1, posX + r); x++) {
			dx = posX - x; // horizontal offset
			dy = posY - y; // vertical offset
			if ((dx*dx + dy * dy) <= (r*r))
			{
				maskTexture.setPixel(x, y, 255);
			}
		}
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if(!simpleTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleTexProgram.log() << endl << endl;
	}
	simpleTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	vShader.initFromFile(VERTEX_SHADER, "shaders/maskedTexture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/maskedTexture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	maskedTexProgram.init();
	maskedTexProgram.addShader(vShader);
	maskedTexProgram.addShader(fShader);
	maskedTexProgram.link();
	if(!maskedTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << maskedTexProgram.log() << endl << endl;
	}
	maskedTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}

void Scene::initLevels()
{
	actualLevel = 0;

	Level firstLevel;
	firstLevel.name = "Just dig!";
	firstLevel.lemmingsToSpawn = 10;
	firstLevel.lemmingsToSave = 1;
	firstLevel.availableTime = 5 * 60;
	firstLevel.spawnPosition = glm::vec2(60, 30);
	firstLevel.savePosition = glm::vec2(100, 50); // TODO: Must adjust this position (randomly selected)
	firstLevel.colorTextureFile = "images/fun1.png";
	firstLevel.maskTextureFile = "images/fun1_mask.png";
	levels.push_back(firstLevel);

	Level secondLevel;
	secondLevel.name = "Only floaters can survive this";
	secondLevel.lemmingsToSpawn = 10;
	secondLevel.lemmingsToSave = 1;
	secondLevel.availableTime = 5 * 60;
	secondLevel.spawnPosition = glm::vec2(30, 15);
	secondLevel.savePosition = glm::vec2(100, 50); // TODO: Must adjust this position (randomly selected)
	secondLevel.colorTextureFile = "images/fun2.png";
	secondLevel.maskTextureFile = "images/fun2_mask.png";
	levels.push_back(secondLevel);
}

void Scene::changeLevel(int newLevel)
{
	actualLevel = newLevel;

	Level level = levels[actualLevel];

	colorTexture.loadFromFile(level.colorTextureFile, TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile(level.maskTextureFile, TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	lemmings.clear();

	spawnedLemmings = 0;

	for (int i = 0; i < level.lemmingsToSpawn; ++i) {
		Lemming * lem = new Lemming();
		lem->init(level.spawnPosition, simpleTexProgram);
		lem->setMapMask(&maskTexture);

		lemmings.push_back(lem);
	}
}

