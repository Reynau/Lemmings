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
	currentLevel = 7;	// From 1 to NumLevels

	currentLevel--;		// Here gets the value in the vector

	initLevels();

	Level level = levels[currentLevel];

	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT))};
	glm::vec2 texCoords[2] = {glm::vec2(level.offset / level.spriteWidth, 0.f), glm::vec2((level.offset + 320.f) / level.spriteWidth, 160.f / 256.0f)};

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

	buttonTexture.loadFromFile("images/lemming_buttons.png", TEXTURE_PIXEL_FORMAT_RGBA);
	buttonTexture.setMinFilter(GL_NEAREST);
	buttonTexture.setMagFilter(GL_NEAREST);

	initButtons();

	sceneSpeed = NORMAL;

	cursor = new Cursor();
	cursor->initCursor(simpleTexProgram, &lemmingTexture);
	index_selected_lem = NULL;
	selected_lem_state = Lemming::NO_SKILL;

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
	int auxdeltaTime = deltaTime;
	deltaTime = considerSceneSpeed(deltaTime);
	currentTime += deltaTime;
	Level level = levels[currentLevel];
	updateColliders();

	// Spawn lemmings
	if (lemmingHasToSpawn(deltaTime)) ++spawnedLemmings;

	// Update lemmings
	for (int i = 0; i < spawnedLemmings; ++i) {
		if (!lemmings[i]) continue;		
		lemmings[i]->update(deltaTime, int(level.offset), colliders);
		if (countds[i]->update(lemmings[i]->getPosition(), deltaTime)) exploteLemming(i);
		if (lemmings[i]->isDead()) removeLemming(i);
		else if (lemmings[i]->isSafe()) {
			removeLemming(i);
			if (countds[i]) removeCount(i);
			++safeLemmings;
		}
		else checkIfLemmingSafe(i);
	}

	for (int i = 0; i < 12; ++i) {
		if (buttons[i]) buttons[i]->update(auxdeltaTime);
	}

	// A level finishes when all lemmings are not alive
	if (aliveLemmings == 0) {
		finishLevel();
	}

	if (cursor->getPos().x < -4.f) moveMap(0);
	else if (cursor->getPos().x > 307.f) moveMap(1);
	spawnDoor->update(deltaTime, int(currentTime/1000));
	door->update(deltaTime, 0);
	if (!checkSelecting()) cursor->setSelect(Cursor::NORMAL);
	checkButtons();
	
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

	for (int i = 0; i < 12; ++i) {
		if (buttons[i]) buttons[i]->render();
	}
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
		if (index_selected_but == 0) applySkill(selected_lem_state);
		else pressButton();
	}
	if (bRightButton) {
		
	}
	cursor->setPos(mouseX, mouseY);
}

void Scene::moveMap(bool right)
{
	
	if (right && levels[currentLevel].offset <= levels[currentLevel].spriteWidth - 326.f) {
		levels[currentLevel].offset += 6.0f;
		levels[currentLevel].spawnPosition.x -= 6;
		levels[currentLevel].savePosition.x -= 6;

		Level level = levels[currentLevel];

		glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
		glm::vec2 texCoords[2] = { glm::vec2(level.offset / level.spriteWidth, 0.f), glm::vec2((level.offset + 320.f) / level.spriteWidth, 160.f / 256.0f) };

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
		glm::vec2 texCoords[2] = { glm::vec2(level.offset / level.spriteWidth, 0.f), glm::vec2((level.offset + 320.f) / level.spriteWidth, 160.f / 256.0f) };

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
	// FUN
	Level firstLevel;
	firstLevel.name = "Just dig!";
	firstLevel.lemmingsToSpawn = 10;
	firstLevel.lemmingsToSecure = 1;
	firstLevel.availableTime = 5 * 60;
	firstLevel.offset = 120.f;
	firstLevel.spawnPosition = glm::vec2(-30 + firstLevel.offset, 30);
	firstLevel.savePosition = glm::vec2(96 + firstLevel.offset, 84);
	firstLevel.colorTextureFile = "images/fun1.png";
	firstLevel.maskTextureFile = "images/fun1_mask.png";
	firstLevel.releaseRate = 50.f;
	firstLevel.door = Door::FIRST_DOOR;
	firstLevel.spriteWidth = 512.f;
	levels.push_back(firstLevel);

	Level secondLevel;
	secondLevel.name = "Only floaters can survive this";
	secondLevel.lemmingsToSpawn = 10;
	secondLevel.lemmingsToSecure = 1;
	secondLevel.availableTime = 5 * 60;
	secondLevel.offset = 69.f;
	secondLevel.spawnPosition = glm::vec2(-41 + secondLevel.offset, 10);
	secondLevel.savePosition = glm::vec2(188 + secondLevel.offset, 101);
	secondLevel.colorTextureFile = "images/fun2.png";
	secondLevel.maskTextureFile = "images/fun2_mask.png";
	secondLevel.releaseRate = 50.f;
	secondLevel.door = Door::SECOND_DOOR;
	secondLevel.spriteWidth = 512.f;
	levels.push_back(secondLevel);

	Level thirdLevel;
	thirdLevel.name = "Tailor-made for blockers";
	thirdLevel.lemmingsToSpawn = 50;
	thirdLevel.lemmingsToSecure = 5;
	thirdLevel.availableTime = 5 * 60;
	thirdLevel.offset = 22.f;
	thirdLevel.spawnPosition = glm::vec2(107 + thirdLevel.offset, 3);
	thirdLevel.savePosition = glm::vec2(73 + thirdLevel.offset, 105);
	thirdLevel.colorTextureFile = "images/fun3.png";
	thirdLevel.maskTextureFile = "images/fun3_mask.png";
	thirdLevel.releaseRate = 50.f;
	thirdLevel.door = Door::SECOND_DOOR;
	thirdLevel.spriteWidth = 512.f;
	levels.push_back(thirdLevel);


	Level fourthLevel;
	fourthLevel.name = "Now use miners and climbers";
	fourthLevel.lemmingsToSpawn = 10;
	fourthLevel.lemmingsToSecure = 10;
	fourthLevel.availableTime = 5 * 60;
	fourthLevel.offset = 122.f;
	fourthLevel.spawnPosition = glm::vec2(-37 + fourthLevel.offset, 10);
	fourthLevel.savePosition = glm::vec2(121 + fourthLevel.offset, 0);
	fourthLevel.colorTextureFile = "images/fun4.png";
	fourthLevel.maskTextureFile = "images/fun4_mask.png";
	fourthLevel.releaseRate = 1.f;
	fourthLevel.door = Door::THIRD_DOOR;
	fourthLevel.spriteWidth = 512.f;
	levels.push_back(fourthLevel);

	// TRICKY
	Level fifthLevel;
	fifthLevel.name = "This should be a doddle!";
	fifthLevel.lemmingsToSpawn = 80;
	fifthLevel.lemmingsToSecure = 40;
	fifthLevel.availableTime = 4 * 60;
	fifthLevel.offset = 22.f;
	fifthLevel.spawnPosition = glm::vec2(70 + fifthLevel.offset, 65);
	fifthLevel.savePosition = glm::vec2(687 + fifthLevel.offset, 69);
	fifthLevel.colorTextureFile = "images/tricky1.png";
	fifthLevel.maskTextureFile = "images/tricky1_mask.png";
	fifthLevel.releaseRate = 50.f;
	fifthLevel.door = Door::FIRST_DOOR;
	fifthLevel.spriteWidth = 924.f;
	levels.push_back(fifthLevel);

	Level sixthLevel;
	sixthLevel.name = "We all fall down";
	sixthLevel.lemmingsToSpawn = 40;
	sixthLevel.lemmingsToSecure = 40;
	sixthLevel.availableTime = 3 * 60;
	sixthLevel.offset = 240.f;
	sixthLevel.spawnPosition = glm::vec2(-78 + sixthLevel.offset, 13);
	sixthLevel.savePosition = glm::vec2(512 + sixthLevel.offset, 72);
	sixthLevel.colorTextureFile = "images/tricky2.png";
	sixthLevel.maskTextureFile = "images/tricky2_mask.png";
	sixthLevel.releaseRate = 1.f;
	sixthLevel.door = Door::FOURTH_DOOR;
	sixthLevel.spriteWidth = 1600.f;
	levels.push_back(sixthLevel);

	Level seventhLevel;
	seventhLevel.name = "A ladder would be handy";
	seventhLevel.lemmingsToSpawn = 80;
	seventhLevel.lemmingsToSecure = 40;
	seventhLevel.availableTime = 6 * 60;
	seventhLevel.offset = 2.f;
	seventhLevel.spawnPosition = glm::vec2(145 + seventhLevel.offset, 1);
	seventhLevel.savePosition = glm::vec2(600 + seventhLevel.offset, 5);
	seventhLevel.colorTextureFile = "images/tricky3.png";
	seventhLevel.maskTextureFile = "images/tricky3_mask.png";
	seventhLevel.releaseRate = 50.f;
	seventhLevel.door = Door::FIRST_DOOR;
	seventhLevel.spriteWidth = 704.f;
	levels.push_back(seventhLevel);

	// TAXING


	// MAYHEM


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

	resetOffsetsAndReleases();

	surrStarted = false;

	Level level = levels[currentLevel];

	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
	glm::vec2 texCoords[2] = { glm::vec2(level.offset / level.spriteWidth, 0.f), glm::vec2((level.offset + 320.f) / level.spriteWidth, 160.f / 256.0f) };

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);

	colorTexture.loadFromFile(level.colorTextureFile, TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile(level.maskTextureFile, TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);

	clearCounts();
	resetLemmings();

	spawnDoor->setState(Door::doorState::SPAWN_DOOR_CLOSE);
	spawnDoor->setPos(glm::vec2(level.spawnPosition.x - (24 - 7), level.spawnPosition.y - (24 - 7)));
	door->setState(level.door);
	door->setPos(glm::vec2(level.savePosition.x, level.savePosition.y));

	resetButtons();
	sceneSpeed = NORMAL;
	selected_lem_state = Lemming::NO_SKILL;

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

void Scene::resetOffsetsAndReleases()
{
	levels[0].offset = 120.f;
	levels[0].releaseRate = 50;
	levels[0].spawnPosition = glm::vec2(-30 + levels[0].offset, 30);
	levels[0].savePosition = glm::vec2(96 + levels[0].offset, 84);
	levels[1].offset = 69.f;
	levels[1].releaseRate = 50;
	levels[1].spawnPosition = glm::vec2(-41 + levels[1].offset, 10);
	levels[1].savePosition = glm::vec2(188 + levels[1].offset, 101);
	levels[2].offset = 22.f;
	levels[2].releaseRate = 50;
	levels[2].spawnPosition = glm::vec2(107 + levels[2].offset, 3);
	levels[2].savePosition = glm::vec2(73 + levels[2].offset, 105);
	levels[3].offset = 122.f;
	levels[3].releaseRate = 1;
	levels[3].spawnPosition = glm::vec2(-37 + levels[3].offset, 10);
	levels[3].savePosition = glm::vec2(121 + levels[3].offset, 0);
	levels[4].offset = 22.f;
	levels[4].releaseRate = 50;
	levels[4].spawnPosition = glm::vec2(70 + levels[4].offset, 65);
	levels[4].savePosition = glm::vec2(687 + levels[4].offset, 69);
	levels[5].offset = 240.f;
	levels[5].releaseRate = 1;
	levels[5].spawnPosition = glm::vec2(-78 + levels[5].offset, 13);
	levels[5].savePosition = glm::vec2(512 + levels[5].offset, 72);
}

bool Scene::checkSelecting()
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
					return true;
				}
			}
		}
	}
	index_selected_lem = 0;
	return false;
}

void Scene::checkButtons()
{
	for (int i = 0; i < 12; ++i) {
		if (!buttons[i]) continue;

		glm::vec2 curPos = cursor->getPos();
		curPos += glm::vec2(7, 7);
		glm::vec2 butPos = buttons[i]->getPos();
		for (int y = int(curPos.y) - 14; y <= int(curPos.y) + 14; y++) {
			for (int x = int(curPos.x) - 12; x <= int(curPos.x) + 12; x++) {
				if (glm::vec2(int(butPos.x) + 12, int(butPos.y) + 14) == glm::vec2(x, y)) {
					index_selected_but = i + 1;
					selected_but = index_selected_but;
					return;
				}
			}
		}
	}
	index_selected_but = 0;
}

void Scene::applyButtonPressed(int i)
{
	if (!(buttons[i]->isSelect())) {
		if (i == 1) {
			selected_lem_state = Lemming::CLIMBER;
		}
		else if (i == 2) {
			selected_lem_state = Lemming::FLOATER;
		}
		else if (i == 3) {
			selected_lem_state = Lemming::SURREND;
		}
		else if (i == 4) {
			selected_lem_state = Lemming::BLOCKER;
		}
		else if (i == 5) {
			selected_lem_state = Lemming::NO_SKILL;
		}
		else if (i == 6) {
			selected_lem_state = Lemming::BASHER;
		}
		else if (i == 7) {
			selected_lem_state = Lemming::NO_SKILL;
		}
		else if (i == 8) {
			selected_lem_state = Lemming::DIGGER;
		}
		else if (i == 9)			// Pressed Pause
				sceneSpeed = PAUSE;
		else if (i == 10) {
			setSurrender();
		}
		else if (i == 11)	// Pressed Fast
			if (!(buttons[9]->isSelect()))
				sceneSpeed = FAST;
	}
}

void Scene::undoButtonPressed(int i)
{
	if (buttons[i]->isSelect()) {
		if (i == 9)			// Unpressed Pause
			if (!(buttons[11]->isSelect()))
				sceneSpeed = NORMAL;
			else sceneSpeed = FAST;
		else if (i == 11)	// Unpressed Fast
			if (!(buttons[9]->isSelect()))
				sceneSpeed = NORMAL;
	}
}

void Scene::applySkill(Lemming::LemmingSkill skill)
{
	if (index_selected_lem == 0) return;
	if (skill == Lemming::LemmingSkill::SURREND) countdownLemming(index_selected_lem - 1);
	else if (!lemmings[index_selected_lem-1]->setSkill(skill)) return;
	//--availableSkills[skill]; 
}

void Scene::pressButton()
{
	if (index_selected_but == 0) return;	// IS IT NECESSARY?
	if (index_selected_but == 1) return;	// CHANGE THIS FOR INCREASE/DECREASE LEMMING SPAWN RATING 
	else {
		if ((index_selected_but - 1) < 9) {
			for (int i = 1; i < 9; i++) {
				if (i == (index_selected_but - 1)) {
					applyButtonPressed(i);
					buttons[i]->setSelect(true);
				}
				else buttons[i]->setSelect(false);
			}
		}
		else {
			for (int i = 9; i < 12; i++) {
				if (i == (index_selected_but - 1))
					if (!(buttons[i]->isSelect())) {
						applyButtonPressed(i);
						buttons[i]->setSelect(true);
					}
					else if ((index_selected_but - 1) != 10) {
						undoButtonPressed(i);
						buttons[i]->setSelect(false);
					}
			}
		}
	}
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

bool Scene::lemmingHasToSpawn(int deltaTime) {
	Level level = levels[currentLevel];
	int ct = currentTime;
	int dt = deltaTime;
	if (sceneSpeed == FAST) {
		ct -= dt;
		dt /= 6;
		ct += dt;
	}
	float spawnTime = getSpawnTime(level.releaseRate);
	int sec = int(ct / 1000 / spawnTime) - 2 * (2/round(spawnTime));

	cout << sec << endl;

	return spawnedLemmings < level.lemmingsToSpawn && spawnedLemmings <= sec && !surrStarted;
}

float Scene::getSpawnTime(int releaseRate)
{
	return (-2.f + (6.f / (1.f + float(pow((releaseRate / 77.f), 1.5f)))));
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

void Scene::resetButtons() {
	for (int i = 0; i < 12; ++i) {
		buttons[i]->setSelect(false);
	}
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
}

void Scene::countdownLemming(int i)
{
	if (!lemmings[i]) return;
	countds[i]->setOn();
}

void Scene::exploteLemming(int i) 
{
	if (!lemmings[i]) return;
	if (!lemmings[i]->setSkill(Lemming::LemmingSkill::SURREND))
		return;
}

void Scene::initButtons() {
	for (int i = 0; i < 12; ++i) {
		Button * but = new Button();
		but->initButton(simpleTexProgram, &buttonTexture);
		but->setPos((i / 12.f) * 320.f, float(CAMERA_HEIGHT + 10));
		if (i == 0)
			but->setAnim(Button::INCREASE_DECREASE_RATING);
		else if (i == 1) 
			but->setAnim(Button::NORMAL_CLIMBER);
		else if (i == 2)
			but->setAnim(Button::NORMAL_FLOATER);
		else if (i == 3)
			but->setAnim(Button::NORMAL_EXPLOSIVE);
		else if (i == 4)
			but->setAnim(Button::NORMAL_BLOCKER);
		else if (i == 5)
			but->setAnim(Button::NORMAL_BUILDER);
		else if (i == 6)
			but->setAnim(Button::NORMAL_BASHER);
		else if (i == 7)
			but->setAnim(Button::NORMAL_DIAG_BASHER);
		else if (i == 8) 
			but->setAnim(Button::NORMAL_DIGGER);
		else if (i == 9)
			but->setAnim(Button::NORMAL_PAUSE);
		else if (i == 10)
			but->setAnim(Button::NORMAL_SURREND);
		else if (i == 11)
			but->setAnim(Button::NORMAL_FAST);
		buttons.push_back(but);
	}
}
