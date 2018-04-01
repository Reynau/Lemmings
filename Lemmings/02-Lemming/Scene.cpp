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

	Level level = levels[currentLevel];

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile(level.colorTextureFile, TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile(level.maskTextureFile, TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	lemmingTexture.loadFromFile("images/lemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
	lemmingTexture.setMinFilter(GL_NEAREST);
	lemmingTexture.setMagFilter(GL_NEAREST);

	initLemmings();

	sceneSpeed = NORMAL;

	cur = new cursor();
	cur->initCursor(simpleTexProgram);


	// TESTING
	lemmings[0]->setState(Lemming::LemmingState::DIGGER_RIGHT_STATE);
}

unsigned int x = 0;

void Scene::update(int deltaTime)
{
	deltaTime = considerSceneSpeed(deltaTime);
	currentTime += deltaTime;

	// Spawn lemmings
	if (lemmingHasToSpawn()) ++spawnedLemmings;

	// Update lemmings
	for (int i = 0; i < spawnedLemmings; ++i) {
		if (!lemmings[i]) continue;
			
		lemmings[i]->update(deltaTime);
		if (lemmings[i]->isDead()) removeLemming(i);
		else checkIfLemmingSafe(i);
	}

	// A level finishes when any lemming is alive
	if (aliveLemmings == 0) {
		finishLevel();
	}
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
		if (lemmings[i]) lemmings[i]->render();
	}

	cur->render();
}

void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	/*if(bLeftButton)
		eraseMask(mouseX, mouseY);
	else if(bRightButton)
		applyMask(mouseX, mouseY);*/
	cur->setPos(mouseX, mouseY);
}

/*void Scene::eraseMask(int mouseX, int mouseY)
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
}*/

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
	currentLevel = 2;

	Level firstLevel;
	firstLevel.name = "Just dig!";
	firstLevel.lemmingsToSpawn = 10;
	firstLevel.lemmingsToSecure = 1;
	firstLevel.availableTime = 5 * 60;
	firstLevel.spawnPosition = glm::vec2(60, 40);
	firstLevel.savePosition = glm::vec2(180, 130); // TODO: Must adjust this position (randomly selected)
	firstLevel.colorTextureFile = "images/fun1.png";
	firstLevel.maskTextureFile = "images/fun1_mask.png";
	levels.push_back(firstLevel);

	Level secondLevel;
	secondLevel.name = "Only floaters can survive this";
	secondLevel.lemmingsToSpawn = 10;
	secondLevel.lemmingsToSecure = 1;
	secondLevel.availableTime = 5 * 60;
	secondLevel.spawnPosition = glm::vec2(30, 15);
	secondLevel.savePosition = glm::vec2(100, 50); // TODO: Must adjust this position (randomly selected)
	secondLevel.colorTextureFile = "images/fun2.png";
	secondLevel.maskTextureFile = "images/fun2_mask.png";
	levels.push_back(secondLevel);

	Level thirdLevel;
	thirdLevel.name = "Tailor-made for blockers";
	thirdLevel.lemmingsToSpawn = 50;
	thirdLevel.lemmingsToSecure = 5;
	thirdLevel.availableTime = 5 * 60;
	thirdLevel.spawnPosition = glm::vec2(50, 15);
	thirdLevel.savePosition = glm::vec2(100, 50); // TODO: Must adjust this position (randomly selected)
	thirdLevel.colorTextureFile = "images/fun3.png";
	thirdLevel.maskTextureFile = "images/fun3_mask.png";
	levels.push_back(thirdLevel);
}

void Scene::changeLevel(int newLevel)
{
	currentLevel = newLevel;

	Level level = levels[currentLevel];

	colorTexture.loadFromFile(level.colorTextureFile, TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile(level.maskTextureFile, TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	resetLemmings();

	currentTime = 0;
}

void Scene::finishLevel() {
	Level level = levels[currentLevel];

	// TODO: Improve this logic
	if (safeLemmings >= level.lemmingsToSecure) {
		// Win
		changeLevel(++currentLevel);
	}
	else {
		// Lose
		changeLevel(currentLevel);
	}
}

void Scene::checkIfLemmingSafe(int lemmingId) {
	Level level = levels[currentLevel];

	glm::vec2 safeSquare = glm::vec2(20, 20); // TODO: Adjust safe square

	glm::vec2 initialPoint = level.savePosition - safeSquare;
	glm::vec2 endingPoint = level.savePosition + safeSquare;

	Lemming * lemming = lemmings[lemmingId];
	if (lemmingColideWith(lemming, initialPoint, endingPoint)) {
		removeLemming(lemmingId);
		--aliveLemmings;
		++safeLemmings;
	}
}

bool Scene::lemmingHasToSpawn() {
	Level level = levels[currentLevel];
	int sec = int(currentTime / 1000);

	return spawnedLemmings < level.lemmingsToSpawn && spawnedLemmings <= sec;
}

bool Scene::lemmingColideWith(Lemming * lemming, glm::vec2 startPoint, glm::vec2 endPoint) {
	glm::vec2 lemmingPos = lemming->getPosition();
	lemmingPos.x = lemmingPos.x + 120;

	// TODO: Improve collision detection
	return (lemmingPos.x > startPoint.x  && lemmingPos.x < endPoint.x && lemmingPos.y > startPoint.y && lemmingPos.y < endPoint.y);
}

void Scene::initLemmings() {
	Level level = levels[currentLevel];
	for (int i = 0; i < level.lemmingsToSpawn; ++i) {
		Lemming * lem = new Lemming();
		lem->init(level.spawnPosition, simpleTexProgram, &lemmingTexture);
		lem->setMapMask(&maskTexture);

		lemmings.push_back(lem);
	}

	spawnedLemmings = 0;
	safeLemmings = 0;
	aliveLemmings = level.lemmingsToSpawn;
}

void Scene::resetLemmings() {
	clearLemmings();
	initLemmings();
}

void Scene::clearLemmings() {
	for (int i = 0; i < lemmings.size(); ++i) {
		if (lemmings[i]) removeLemming(i);
	}
	lemmings.clear();
}

void Scene::removeLemming(int lemmingId) {
	lemmings[lemmingId]->remove();
	delete (lemmings[lemmingId]);
	lemmings[lemmingId] = NULL;
}

int Scene::considerSceneSpeed(int deltaTime) {
	switch (sceneSpeed) {
	case PAUSE:
		return 0;
	case NORMAL:
		return deltaTime;
	case FAST:
		return 2 * deltaTime;
	}
	return deltaTime;
}