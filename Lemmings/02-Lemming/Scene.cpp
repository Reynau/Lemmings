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
	glm::vec2 texCoords[2] = {glm::vec2(level.offset / 512.0f, 0.f), glm::vec2((level.offset + 320.f) / 512.0f, 160.f / 256.0f)};

	initShaders();

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile(level.colorTextureFile, TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile(level.maskTextureFile, TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	maskTexture2 = &maskTexture;

	quad = Quad::createQuad(-20.f, 0.f, 20.f, float(CAMERA_HEIGHT + 40), program);
	quadui = Quad::createQuad(0.f, 0.f, float(CAMERA_WIDTH), 41.f, program);

	projection = glm::ortho(-20.f, float(CAMERA_WIDTH + 20), float(CAMERA_HEIGHT + 40), 0.f);
	currentTime = 0.0f;

	lemmingTexture.loadFromFile("images/lemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
	lemmingTexture.setMinFilter(GL_NEAREST);
	lemmingTexture.setMagFilter(GL_NEAREST);

	initLemmings();

	sceneSpeed = NORMAL;

	cursor = new Cursor();
	cursor->initCursor(simpleTexProgram, &lemmingTexture);
	index_selected_lem = NULL;

	spawnDoor = new Door();
	spawnDoor->initDoor(simpleTexProgram);
	spawnDoor->setState(Door::doorState::SPAWN_DOOR_CLOSE);
	spawnDoor->setPos(glm::vec2(level.spawnPosition.x - (24 - 7), level.spawnPosition.y - (24 - 7)));
	door = new Door();
	door->initDoor(simpleTexProgram);
	door->setState(level.door);
	door->setPos(glm::vec2(level.savePosition.x, level.savePosition.y));
}

unsigned int x = 0;

void Scene::update(int deltaTime)
{
	deltaTime = considerSceneSpeed(deltaTime);
	currentTime += deltaTime;
	Level level = levels[currentLevel];
	updateColliders();

	// Spawn lemmings
	if (lemmingHasToSpawn()) ++spawnedLemmings;

	if (int(currentTime / 100) == 100 && !surrStarted) setSurrender();

	// Update lemmings
	for (int i = 0; i < spawnedLemmings; ++i) {
		if (!lemmings[i]) continue;		
		lemmings[i]->update(deltaTime, int(level.offset), colliders);
		if (surrStarted) {
			if (countds[i]->update(lemmings[i]->getPosition(), deltaTime)) {
				setSurrender();
			}
		}
		if (lemmings[i]->isDead()) removeLemming(i);
		else if (lemmings[i]->isSafe()) {
			removeLemming(i);
			++safeLemmings;
		}
		else checkIfLemmingSafe(i);
	}

	// A level finishes when all lemmings are not alive
	if (aliveLemmings == 0) {
		finishLevel();
	}

	if (cursor->getPos().x < -4.f) moveMap(0);
	else if (cursor->getPos().x > 307.f) moveMap(1);
	spawnDoor->update(deltaTime, int(currentTime/1000));
	door->update(deltaTime, 0);
	checkSelecting();
}

void Scene::render()
{

	glm::mat4 modelview;

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	spawnDoor->render();
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
		if (countds[i]) countds[i]->render();
	}

	program.use();
	program.setUniformMatrix4f("projection", projection);
	program.setUniform4f("color", 0.f, 0.f, 0.f, 1.0f);

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, float(CAMERA_HEIGHT - 1), 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	quadui->render();

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);
	cursor->render();

	program.use();
	program.setUniformMatrix4f("projection", projection);
	program.setUniform4f("color", 0.f, 0.f, 0.f, 1.0f);

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	quad->render();

	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(float(CAMERA_WIDTH + 20), 0.f, 0.f));
	program.setUniformMatrix4f("modelview", modelview);
	quad->render();

	
}

void Scene::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton)
{
	if (bLeftButton) {
		applySkill(Lemming::LemmingSkill::DIGGER); // TESTING
	}
	if (bRightButton) {
		applySkill(Lemming::LemmingSkill::BLOCKER);
	}
	cursor->setPos(mouseX, mouseY);
}

void Scene::moveMap(bool right)
{
	if (right && levels[currentLevel].offset <= 186.f) {
		levels[currentLevel].offset += 6.0f;
		levels[currentLevel].spawnPosition.x -= 6;
		levels[currentLevel].savePosition.x -= 6;

		Level level = levels[currentLevel];

		glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
		glm::vec2 texCoords[2] = { glm::vec2(level.offset / 512.0, 0.f), glm::vec2((level.offset + 320.f) / 512.0f, 160.f / 256.0f) };

		initShaders();

		map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);

		spawnDoor->setPos(glm::vec2(level.spawnPosition.x - (24 - 7), level.spawnPosition.y - (24 - 7)));
		door->setPos(glm::vec2(level.savePosition.x, level.savePosition.y));
		for (int i = 0; i < level.lemmingsToSpawn; ++i) {
			if (!lemmings[i]) continue;
			lemmings[i]->setPos(glm::vec2(lemmings[i]->getPosition().x - 6.f, lemmings[i]->getPosition().y));
		}
	}
	else if (!right && levels[currentLevel].offset >= 6.f) {
		levels[currentLevel].offset -= 6.0f;
		levels[currentLevel].spawnPosition.x += 6;
		levels[currentLevel].savePosition.x += 6;

		Level level = levels[currentLevel];

		glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
		glm::vec2 texCoords[2] = { glm::vec2(level.offset / 512.0, 0.f), glm::vec2((level.offset + 320.f) / 512.0f, 160.f / 256.0f) };

		initShaders();

		map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);

		spawnDoor->setPos(glm::vec2(level.spawnPosition.x - (24 - 7), level.spawnPosition.y - (24 - 7)));
		door->setPos(glm::vec2(level.savePosition.x, level.savePosition.y));

		for (int i = 0; i < level.lemmingsToSpawn; ++i) {
			if (!lemmings[i]) continue;
			lemmings[i]->setPos(glm::vec2(lemmings[i]->getPosition().x + 6.f, lemmings[i]->getPosition().y));
		}
	}
}

void Scene::initShaders()
{
	Shader vShader, fShader;

	/////
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
	//////

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
	firstLevel.spawnPosition = glm::vec2(90, 30); // Correct
	firstLevel.savePosition = glm::vec2(216, 84); // Correct
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
	secondLevel.spawnPosition = glm::vec2(28, 10); // Correct
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
	thirdLevel.spawnPosition = glm::vec2(129, 3); // Correct
	thirdLevel.savePosition = glm::vec2(95, 105); // Correct
	thirdLevel.colorTextureFile = "images/fun3.png";
	thirdLevel.maskTextureFile = "images/fun3_mask.png";
	thirdLevel.offset = 22.f;
	thirdLevel.spawnTime = 2;
	thirdLevel.door = Door::SECOND_DOOR;
	levels.push_back(thirdLevel);
}


void Scene::updateColliders() {
	Level level = levels[currentLevel];
	// Lemmings
	colliders.clear();
	for (int i = 0; i < spawnedLemmings; ++i) {
		if (!lemmings[i]) continue;

		if (lemmings[i]->isBlocker()) {
			glm::vec2 position = lemmings[i]->getPosition();
			colliders.push_back(position + glm::vec2(2 + level.offset, 4));
			colliders.push_back(position + glm::vec2(13 + level.offset, 11));
		}
	}
}

void Scene::changeLevel(int newLevel)
{
	currentLevel = newLevel;

	resetOffsets();

	surrStarted = false;

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

	spawnDoor->setState(Door::doorState::SPAWN_DOOR_CLOSE);
	spawnDoor->setPos(glm::vec2(level.spawnPosition.x - (24 - 7), level.spawnPosition.y - (24 - 7)));
	door->setState(level.door);
	door->setPos(glm::vec2(level.savePosition.x, level.savePosition.y));

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

void Scene::resetOffsets()
{
	levels[0].offset = 120.f;
	levels[0].spawnPosition = glm::vec2(90, 30);
	levels[0].savePosition = glm::vec2(216, 85);
	levels[1].offset = 69.f;
	levels[1].spawnPosition = glm::vec2(28, 10);
	levels[1].savePosition = glm::vec2(257, 101);
	levels[2].offset = 22.f;
	levels[2].spawnPosition = glm::vec2(129, 3);
	levels[2].savePosition = glm::vec2(95, 105);
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

	glm::vec2 safeSquare = glm::vec2(3, 3); // TODO: Adjust safe square

	glm::vec2 initialPoint = level.savePosition - safeSquare;
	glm::vec2 endingPoint = level.savePosition + safeSquare;

	// Set the safeSquare into the door
	initialPoint.x += 24 - safeSquare.x / 2;
	endingPoint.x += 24 - safeSquare.x / 2;
	initialPoint.y += 48 - safeSquare.y / 2;
	endingPoint.y += 48 - safeSquare.y / 2;

	Lemming * lemming = lemmings[lemmingId];
	if (!lemmings[lemmingId]->isArriving()) {
		if (lemmingColideWith(lemming, initialPoint, endingPoint)) {
			lemmings[lemmingId]->lemmingArrived();
		}
	}
}

bool Scene::lemmingHasToSpawn() {
	Level level = levels[currentLevel];
	int sec = int(currentTime / 1000 / level.spawnTime) - 2;

	return spawnedLemmings < level.lemmingsToSpawn && spawnedLemmings <= sec && !surrStarted;
}

bool Scene::lemmingColideWith(Lemming * lemming, glm::vec2 startPoint, glm::vec2 endPoint) {
	Level level = levels[currentLevel];

	glm::vec2 lemmingPos = lemming->getPosition();
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

		Countdown * count = new Countdown();
		count->initCountdown(level.spawnPosition, simpleTexProgram, &lemmingTexture);
		countds.push_back(count);
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

void Scene::clearCounts() {
	for (int i = 0; i < int(countds.size()); ++i) {
		if (countds[i]) removeCount(i);
	}
	countds.clear();
}

void Scene::removeLemming(int lemmingId) {
	lemmings[lemmingId]->remove();
	delete (lemmings[lemmingId]);
	lemmings[lemmingId] = NULL;
	--aliveLemmings;
}

void Scene::removeCount(int countId) {
	countds[countId]->remove();
	delete (countds[countId]);
	countds[countId] = NULL;
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
void Scene::setSurrender()
{
	if (!surrStarted) {
		surrStarted = true;
		aliveLemmings = 0;
		for (int i = 0; i < spawnedLemmings; ++i) {
			if (!lemmings[i]) continue;
			countds[i]->setOn();
			aliveLemmings++;
		}
	}
	else {
		for (int i = 0; i < spawnedLemmings; ++i) {
			if (!lemmings[i]) continue;
			if (!lemmings[i]->setSkill(Lemming::LemmingSkill::SURREND))
				continue;
		}
		// aliveLemmings = 0;
	}	
}