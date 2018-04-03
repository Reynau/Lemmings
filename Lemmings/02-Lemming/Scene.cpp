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
	//SELECT LEVEL
	currentLevel = 0;

	initLevels();

	Level level = levels[currentLevel];

	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT))};
	glm::vec2 texCoords[2] = {glm::vec2(level.offset / 512.0, 0.f), glm::vec2((level.offset + 320.f) / 512.0f, 160.f / 256.0f)};

	initShaders();

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

	sceneSpeed = FAST;

	cursor = new Cursor();
	cursor->initCursor(simpleTexProgram);
	index_selected_lem = NULL;

	door = new Door();
	door->initDoor(simpleTexProgram);
	door->setState(level.door);
	door->setPos(glm::vec2(level.savePosition.x/* + level.offset*/, level.savePosition.y));
}

unsigned int x = 0;

void Scene::update(int deltaTime)
{
	deltaTime = considerSceneSpeed(deltaTime);
	currentTime += deltaTime;
	Level level = levels[currentLevel];

	// Spawn lemmings
	if (lemmingHasToSpawn()) ++spawnedLemmings;

	// Update lemmings
	for (int i = 0; i < spawnedLemmings; ++i) {
		if (!lemmings[i]) continue;
			
		lemmings[i]->update(deltaTime, int(level.offset));
		if (lemmings[i]->isDead()) removeLemming(i);
		else checkIfLemmingSafe(i);
	}

	// A level finishes when all lemmings are not alive
	if (aliveLemmings == 0) {
		finishLevel();
	}

	door->update(deltaTime);
	checkSelecting();
}

void Scene::render()
{

	glm::mat4 modelview;

	door->render();

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

	cursor->render();
}

void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	if(bLeftButton)
		applySkill(Lemming::DIGGER); // TESTING
	/*else if(bRightButton)
		applyMask(mouseX, mouseY);*/
	cursor->setPos(mouseX, mouseY);
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
	Level firstLevel;
	firstLevel.name = "Just dig!";
	firstLevel.lemmingsToSpawn = 10;
	firstLevel.lemmingsToSecure = 1;
	firstLevel.availableTime = 5 * 60;
	firstLevel.spawnPosition = glm::vec2(60, 40);
	firstLevel.savePosition = glm::vec2(216, 85); // TODO: Must adjust this position (randomly selected)
	firstLevel.colorTextureFile = "images/fun1.png";
	firstLevel.maskTextureFile = "images/fun1_mask.png";
	firstLevel.offset = 120.f;
	firstLevel.spawnTime = 2;
	firstLevel.door = Door::FIRST_DOOR;
	levels.push_back(firstLevel);

	Level secondLevel;
	secondLevel.name = "Only floaters can survive this";
	secondLevel.lemmingsToSpawn = 10;
	secondLevel.lemmingsToSecure = 1;
	secondLevel.availableTime = 5 * 60;
	secondLevel.spawnPosition = glm::vec2(28, 15); // Correct
	secondLevel.savePosition = glm::vec2(257, 101); // Correct
	secondLevel.colorTextureFile = "images/fun2.png";
	secondLevel.maskTextureFile = "images/fun2_mask.png";
	secondLevel.offset = 69.f;
	secondLevel.spawnTime = 2;
	secondLevel.door = Door::SECOND_DOOR;
	levels.push_back(secondLevel);

	Level thirdLevel;
	thirdLevel.name = "Tailor-made for blockers";
	thirdLevel.lemmingsToSpawn = 50;
	thirdLevel.lemmingsToSecure = 5;
	thirdLevel.availableTime = 5 * 60;
	thirdLevel.spawnPosition = glm::vec2(129, 10); // Correct
	thirdLevel.savePosition = glm::vec2(95, 105); // Correct
	thirdLevel.colorTextureFile = "images/fun3.png";
	thirdLevel.maskTextureFile = "images/fun3_mask.png";
	thirdLevel.offset = 22.f;
	thirdLevel.spawnTime = 2;
	thirdLevel.door = Door::SECOND_DOOR;
	levels.push_back(thirdLevel);
}

void Scene::changeLevel(int newLevel)
{
	currentLevel = newLevel;

	Level level = levels[currentLevel];

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
	glm::vec2 texCoords[2] = { glm::vec2(level.offset / 512.0, 0.f), glm::vec2((level.offset + 320.f) / 512.0f, 160.f / 256.0f) };

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);

	colorTexture.loadFromFile(level.colorTextureFile, TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile(level.maskTextureFile, TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	resetLemmings();

	level.spawnTime = 2;

	door->setState(level.door);
	door->setPos(glm::vec2(level.savePosition.x/* + level.offset*/, level.savePosition.y));

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

void Scene::checkSelecting()
{
	for (int i = 0; i < spawnedLemmings; ++i) {
		if (!lemmings[i]) continue;

		glm::vec2 curPos = cursor->getPos();
		glm::vec2 lemPos = lemmings[i]->getPosition();
		for (int y = int(curPos.y); y <= int(curPos.y) + 10; y++) {
			for (int x = int(curPos.x) + 4; x <= int(curPos.x) + 10; x++) {
				if (glm::vec2(int(lemPos.x) + 7, int(lemPos.y) + 10) == glm::vec2(x, y)) {
					cursor->setSelect(Cursor::SELECT);
					index_selected_lem = i+1;
					return;
				}
			}
		}
	}

	cursor->setSelect(Cursor::NORMAL);
	index_selected_lem = NULL;
}

void Scene::applySkill(Lemming::LemmingSkill skill)
{
	if (!index_selected_lem) return;
	// TESTING
	if (!lemmings[index_selected_lem-1]->setSkill(skill)) return;
	//--availableSkills[skill]; 
}

void Scene::checkIfLemmingSafe(int lemmingId) {
	Level level = levels[currentLevel];

	glm::vec2 safeSquare = glm::vec2(2, 2); // TODO: Adjust safe square

	glm::vec2 initialPoint = level.savePosition - safeSquare;
	glm::vec2 endingPoint = level.savePosition + safeSquare;

	// Set the safeSquare into the door
	initialPoint.x += 24 - safeSquare.x / 2;
	endingPoint.x += 24 - safeSquare.x / 2;
	initialPoint.y += 48 - safeSquare.y / 2;
	endingPoint.y += 48 - safeSquare.y / 2;

	//if (!lemmings[lemmingId]) return; NO ARREGLA EL ERROR
	Lemming * lemming = lemmings[lemmingId];
	if (lemmingColideWith(lemming, initialPoint, endingPoint)) {
		removeLemming(lemmingId);
		--aliveLemmings;
		++safeLemmings;
	}
}

bool Scene::lemmingHasToSpawn() {
	Level level = levels[currentLevel];
	int sec = int(currentTime / 1000 / level.spawnTime);

	return spawnedLemmings < level.lemmingsToSpawn && spawnedLemmings <= sec;
}

bool Scene::lemmingColideWith(Lemming * lemming, glm::vec2 startPoint, glm::vec2 endPoint) {
	Level level = levels[currentLevel];

	glm::vec2 lemmingPos = glm::vec2(lemming->getPosition().x, lemming->getPosition().y);
	lemmingPos += glm::ivec2(7, 15);
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
	for (int i = 0; i < int(lemmings.size()); ++i) {
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
		return 3 * deltaTime;
	}
	return deltaTime;
}