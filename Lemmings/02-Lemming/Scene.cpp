#include <iostream>
#include <cmath>
#include <algorithm>
#include <GL/glew.h>
#include <GL/glut.h>
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
	audioDriver.removeAudio("sounds/MOUSEPRE.WAV");
	audioDriver.removeAudio("sounds/YIPPEE.WAV");
	audioDriver.removeAudio("sounds/EXPLODE.WAV");
	audioDriver.removeAudio("sounds/SPLAT.WAV");
	audioDriver.removeAudio("sounds/00_-_Lemmings_-_Let_s_Go_.wav");
	audioDriver.removeAudio("sounds/00_lets_go.wav");
	audioDriver.removeAudio("sounds/01_lemming1.wav");
	audioDriver.removeAudio("sounds/02_lemming2.wav");
	audioDriver.removeAudio("sounds/03_lemming3.wav");
}

void Scene::init(Cursor* cursor)
{
	//SELECT LEVEL
	currentLevel = 1;	// From 1 to NumLevels

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

	this->cursor = cursor;
	this->cursor->initCursor(simpleTexProgram, &lemmingTexture);
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

	if (!text.init("fonts/Pixellari.ttf"))
		cout << "Could not load font!!!" << endl;

	lemmingTime = 0;
	previousTime = 0.f;
	startSpawn = false;
	skillApplied = false;
	previousSkillTime = 0.f;
	exploteTime = 0.f;
	clicked = false;

	audioDriver = Audio();
	goStarted = false;
	letsgoStarted = false;
	musicStarted = false;
	exploteDone = false;
	audioDriver.loadAudio("sounds/MOUSEPRE.WAV");
	audioDriver.loadAudio("sounds/YIPPEE.WAV");
	audioDriver.loadAudio("sounds/EXPLODE.WAV");
	audioDriver.loadAudio("sounds/SPLAT.WAV");
	audioDriver.loadAudio("sounds/00_-_Lemmings_-_Let_s_Go_.wav");
	audioDriver.loadAudio("sounds/00_lets_go.wav");
	audioDriver.loadAudio("sounds/01_lemming1.wav");
	audioDriver.loadAudio("sounds/02_lemming2.wav");
	audioDriver.loadAudio("sounds/03_lemming3.wav");
}

unsigned int x = 0;

int Scene::update(int deltaTime)
{
	int updateState = 0;
	int auxdeltaTime = deltaTime;
	deltaTime = considerSceneSpeed(deltaTime);
	currentTime += deltaTime;

	if (previousSkillTime + 100 < currentTime)
		skillApplied = false;
	if (exploteTime + 100 < currentTime)
		exploteDone = false;

	if (levelTime <= 0) {
		finishLevel();
		updateState = 7;
	}

	if (currentTime < 2000) {
		wid = float(glutGet(GLUT_WINDOW_WIDTH));
		cout << wid << endl;
		hei = float(glutGet(GLUT_WINDOW_HEIGHT));
		xOffsetIn = roundf(wid * 55.f / 192.f);
		xOffsetOut = roundf(wid * 95.f / 192.f);
		xOffsetTime = roundf(wid * 127.f / 192.f);
		yOffset = roundf(hei * 911.f / 1080.f);
		sizeFont = roundf(hei * 29.f / 540.f);
		sizeFontBut = roundf(hei * 5.f / 108.f);
		xOffsetBut = roundf(wid * 59.f / 800.f);
		yOffsetBut = roundf(hei * 407.f / 450.f);
		xOffsetMult = wid * 118.3f / 1600.f;
	}

	if (!goStarted) {
		audioDriver.playAudio("sounds/00_-_Lemmings_-_Let_s_Go_.wav");
		goStarted = true;
	}

	if (currentTime > 1000 && !letsgoStarted) {
		audioDriver.playAudio("sounds/00_lets_go.wav");
		letsgoStarted = true;
	}

	if ((currentTime > 3000) && !musicStarted) {
		songNum = currentLevel % 3;
		if (songNum == 0) {
			audioDriver.playAudio("sounds/01_lemming1.wav");
		}
		else if (songNum == 1) {
			audioDriver.playAudio("sounds/02_lemming2.wav");
		}
		else if (songNum == 2) {
			audioDriver.playAudio("sounds/03_lemming3.wav");
		}
		musicStarted = true;
	}

	Level level = levels[currentLevel];
	updateColliders();

	// Spawn lemmings
	if (lemmingHasToSpawn(deltaTime)) ++spawnedLemmings;

	// Update lemmings
	for (int i = 0; i < spawnedLemmings; ++i) {
		if (!lemmings[i]) continue;		
		lemmings[i]->update(deltaTime, int(level.offset), colliders);
		if (countds[i]->update(lemmings[i]->getPosition(), deltaTime)) exploteLemming(i);
		if (lemmings[i]->hasExploted() && !exploteDone) {
			exploteDone = true;
			exploteTime = currentTime;
		}
		if (lemmings[i]->isDead()) {
			removeLemming(i);
			if (countds[i]) removeCount(i);
		}
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
		updateState = 7;
	}

	if (cursor->getPos().x < -4.f) moveMap(0);
	else if (cursor->getPos().x > 307.f) moveMap(1);
	spawnDoor->update(deltaTime, int(currentTime/1000));
	door->update(deltaTime, 0);
	if (!checkSelecting()) cursor->setSelect(Cursor::NORMAL);
	
	audioDriver.update();

	return updateState;
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

	program.use();
	program.setUniformMatrix4f("projection", projection);
	program.setUniform4f("color", 0.f, 0.f, 0.f, 1.0f);
	

	Level level = levels[currentLevel];
	percent = safeLemmings * 100 / level.lemmingsToSpawn;
	if ((currentTime / 1000) > 1) levelTime = level.availableTime - ((currentTime / 1000) - 2);
	else levelTime = level.availableTime;


	string out, in, dash, but;
	if (spawnedLemmings < 10) out = "OUT:  ";
	else out = "OUT:";
	if (percent < 10) in = "IN:  ";
	else in = "IN:";

	

	int minutes = levelTime / 60;
	int seconds = levelTime % 60;
	if (seconds < 10) dash = "-0";
	else dash = "-";
	

	text.render(out + to_string(spawnedLemmings), glm::vec2(xOffsetIn - 3, yOffset - 3), sizeFont, glm::vec4(0.74, 0.89, 0.38, 1));
	text.render(in + to_string(percent) + "%", glm::vec2(xOffsetOut -3, yOffset - 3), sizeFont, glm::vec4(0.74, 0.89, 0.38, 1));
	text.render("TIME: " + to_string(minutes) + dash + to_string(seconds), glm::vec2(xOffsetTime - 3, yOffset - 3), sizeFont, glm::vec4(0.74, 0.89, 0.38, 1));

	text.render(out + to_string(spawnedLemmings), glm::vec2(xOffsetIn + 3, yOffset + 3), sizeFont, glm::vec4(0.51, 0.61, 0.26, 1));
	text.render(in + to_string(percent) + "%", glm::vec2(xOffsetOut + 3, yOffset + 3), sizeFont, glm::vec4(0.51, 0.61, 0.26, 1));
	text.render("TIME: " + to_string(minutes) + dash + to_string(seconds), glm::vec2(xOffsetTime + 3, yOffset + 3), sizeFont, glm::vec4(0.51, 0.61, 0.26, 1));

	text.render(out + to_string(spawnedLemmings), glm::vec2(xOffsetIn, yOffset), sizeFont, glm::vec4(0.63, 0.76, 0.32, 1));
	text.render(in + to_string(percent) + "%", glm::vec2(xOffsetOut, yOffset), sizeFont, glm::vec4(0.63, 0.76, 0.32, 1));
	text.render("TIME: " + to_string(minutes) + dash + to_string(seconds), glm::vec2(xOffsetTime, yOffset), sizeFont, glm::vec4(0.63, 0.76, 0.32, 1));
	
	if (int(level.releaseRate) < 10) text.render("0" + to_string(int(level.releaseRate)), glm::vec2(xOffsetBut, yOffsetBut), sizeFontBut, glm::vec4(0.9, 0.9, 0.9, 1));
	else text.render(to_string(int(level.releaseRate)), glm::vec2(xOffsetBut, yOffsetBut), sizeFontBut, glm::vec4(0.9, 0.9, 0.9, 1));

	for (int i = 0; i < 8; i++) {
		if (level.availableSkills[i] < 10) text.render("0" + to_string(level.availableSkills[i]), glm::vec2(roundf(float(xOffsetBut + ((float(i)+1.f) * xOffsetMult))), yOffsetBut), sizeFontBut, glm::vec4(0.9, 0.9, 0.9, 1));
		else text.render(to_string(level.availableSkills[i]), glm::vec2(roundf(float(xOffsetBut + ((float(i) + 1.f) * xOffsetMult))), yOffsetBut), sizeFontBut, glm::vec4(0.9, 0.9, 0.9, 1));
	}

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
	if (!bLeftButton) clicked = false;
	if (!clicked) {
		checkButtons();
		if (bLeftButton) {
			if (index_selected_but == 0) applySkill(selected_lem_state);
			else pressButton();
			clicked = true;
		}
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
	firstLevel.availableSkills = { 0, 0, 0, 0, 0, 0, 0, 10 };
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
	secondLevel.availableSkills = { 0, 10, 0, 0, 0, 0, 0, 0 };
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
	thirdLevel.availableSkills = { 0, 0, 0, 10, 0, 0, 0, 0 };
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
	fourthLevel.availableSkills = { 10, 0, 0, 0, 0, 0, 1, 0 };
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
	fifthLevel.availableSkills = { 10, 10, 10, 10, 10, 10, 10, 10 };
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
	sixthLevel.availableSkills = { 0, 0, 0, 0, 0, 0, 0, 20 };
	levels.push_back(sixthLevel);

	Level seventhLevel;
	seventhLevel.name = "A ladder would be handy";
	seventhLevel.lemmingsToSpawn = 80;
	seventhLevel.lemmingsToSecure = 40;
	seventhLevel.availableTime = 6 * 60;
	seventhLevel.offset = 2.f;
	seventhLevel.spawnPosition = glm::vec2(145 + seventhLevel.offset, 1);
	seventhLevel.savePosition = glm::vec2(626 + seventhLevel.offset, 3);
	seventhLevel.colorTextureFile = "images/tricky3.png";
	seventhLevel.maskTextureFile = "images/tricky3_mask.png";
	seventhLevel.releaseRate = 50.f;
	seventhLevel.door = Door::FIRST_DOOR;
	seventhLevel.spriteWidth = 704.f;
	seventhLevel.availableSkills = { 20, 20, 20, 20, 40, 20, 20, 20 };
	levels.push_back(seventhLevel);

	// TAXING
	Level eighthLevel;
	eighthLevel.name = "If at first you don't succeed..";
	eighthLevel.lemmingsToSpawn = 80;
	eighthLevel.lemmingsToSecure = 78;
	eighthLevel.availableTime = 4 * 60;
	eighthLevel.offset = 2.f;
	eighthLevel.spawnPosition = glm::vec2(50 + eighthLevel.offset, 30);
	eighthLevel.savePosition = glm::vec2(1120 + eighthLevel.offset, 15);
	eighthLevel.colorTextureFile = "images/taxing1.png";
	eighthLevel.maskTextureFile = "images/taxing1_mask.png";
	eighthLevel.releaseRate = 40.f;
	eighthLevel.door = Door::SECOND_DOOR;
	eighthLevel.spriteWidth = 1224.f;
	eighthLevel.availableSkills = { 2, 2, 2, 2, 8, 2, 2, 2 };
	levels.push_back(eighthLevel);

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

	lemmingTime = 0;
	previousTime = 0.f;
	startSpawn = false;

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


	if (songNum == 0) {
		audioDriver.removeAudio("sounds/01_lemming1.wav");
		audioDriver.loadAudio("sounds/01_lemming1.wav");
	}
	else if (songNum == 1) {
		audioDriver.removeAudio("sounds/02_lemming2.wav");
		audioDriver.loadAudio("sounds/02_lemming2.wav");
	}
	else if (songNum == 2) {
		audioDriver.removeAudio("sounds/03_lemming3.wav");
		audioDriver.loadAudio("sounds/03_lemming3.wav");
	}

	currentTime = 0;
	goStarted = false;
	letsgoStarted = false;
	musicStarted = false;
	exploteDone = false;

}

void Scene::finishLevel() {
	Level level = levels[currentLevel];

	lastSavedLemmnings = safeLemmings;
	lastLemmingsToSecure = level.lemmingsToSecure;

	if (safeLemmings >= level.lemmingsToSecure) {	// Win
		winner = true;
		changeLevel(++currentLevel);
	}
	else {	// Lose
		winner = false;
		changeLevel(currentLevel);
	}
}

void Scene::resetOffsetsAndReleases()
{
	levels[0].offset = 120.f;
	levels[0].releaseRate = 50.f;
	levels[0].spawnPosition = glm::vec2(-30 + levels[0].offset, 30);
	levels[0].savePosition = glm::vec2(96 + levels[0].offset, 84);
	levels[0].availableSkills = { 0, 0, 0, 0, 0, 0, 0, 10 };
	levels[1].offset = 69.f;
	levels[1].releaseRate = 50.f;
	levels[1].spawnPosition = glm::vec2(-41 + levels[1].offset, 10);
	levels[1].savePosition = glm::vec2(188 + levels[1].offset, 101);
	levels[1].availableSkills = { 0, 10, 0, 0, 0, 0, 0, 0 };
	levels[2].offset = 22.f;
	levels[2].releaseRate = 50.f;
	levels[2].spawnPosition = glm::vec2(107 + levels[2].offset, 3);
	levels[2].savePosition = glm::vec2(73 + levels[2].offset, 105);
	levels[2].availableSkills = { 0, 0, 0, 10, 0, 0, 0, 0 };
	levels[3].offset = 122.f;
	levels[3].releaseRate = 1.f;
	levels[3].spawnPosition = glm::vec2(-37 + levels[3].offset, 10);
	levels[3].savePosition = glm::vec2(121 + levels[3].offset, 0);
	levels[3].availableSkills = { 10, 0, 0, 0, 0, 0, 1, 0 };
	levels[4].offset = 22.f;
	levels[4].releaseRate = 50.f;
	levels[4].spawnPosition = glm::vec2(70 + levels[4].offset, 65);
	levels[4].savePosition = glm::vec2(687 + levels[4].offset, 69);
	levels[4].availableSkills = { 10, 10, 10, 10, 10, 10, 10, 10 };
	levels[5].offset = 240.f;
	levels[5].releaseRate = 1.f;
	levels[5].spawnPosition = glm::vec2(-78 + levels[5].offset, 13);
	levels[5].savePosition = glm::vec2(512 + levels[5].offset, 72);
	levels[5].availableSkills = { 0, 0, 0, 0, 0, 0, 0, 20 };
	levels[6].offset = 2.f;
	levels[6].releaseRate = 50.f;
	levels[6].spawnPosition = glm::vec2(145 + levels[6].offset, 1);
	levels[6].savePosition = glm::vec2(626 + levels[6].offset, 3);
	levels[6].availableSkills = { 20, 20, 20, 20, 40, 20, 20, 20 };
	levels[7].offset = 2.f;
	levels[7].releaseRate = 40.f;
	levels[7].spawnPosition = glm::vec2(50 + levels[7].offset, 30);
	levels[7].savePosition = glm::vec2(1120 + levels[7].offset, 15);
	levels[7].availableSkills = { 2, 2, 2, 2, 8, 2, 2, 2 };
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
					if (index_selected_but == 1) {
						for (int y = int(curPos.y) - 3; y <= int(curPos.y) + 4; y++) {
							for (int x = int(curPos.x) - 3; x <= int(curPos.x) + 4; x++) {
								if (glm::vec2(int(butPos.x) + 13, int(butPos.y) + 18) == glm::vec2(x, y)) index_selected_release = 1;
								else if (glm::vec2(int(butPos.x) + 13, int(butPos.y) + 26) == glm::vec2(x, y)) index_selected_release = 2;
							}
						}
					}
					else index_selected_release = 0;
					return;
				}
			}
		}
	}
	index_selected_but = 0;
}

void Scene::releaseRateUp()
{
	if (levels[currentLevel].releaseRate < 99.f)
		levels[currentLevel].releaseRate++;
}

void Scene::releaseRateDown()
{
	if (levels[currentLevel].releaseRate > 1.f)
		levels[currentLevel].releaseRate--;
}

int Scene::getLemmingsToSave() {
	return lastLemmingsToSecure;
}

int Scene::getSavedLemmings() {
	return lastSavedLemmnings;
}

bool Scene::isWinner() {
	return winner;
}

void Scene::checkAndSetReleaseButton()
{
	if (index_selected_release == 1) {
		if (levels[currentLevel].releaseRate < 99.f)
			levels[currentLevel].releaseRate++;
	}
	else if (index_selected_release == 2) {
		if (levels[currentLevel].releaseRate > 1.f)
			levels[currentLevel].releaseRate--;
	}
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
			selected_lem_state = Lemming::BUILDER;
		}
		else if (i == 6) {
			selected_lem_state = Lemming::BASHER;
		}
		else if (i == 7) {
			selected_lem_state = Lemming::DIAG_BASHER;
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
	else if (!skillApplied) {
		if (levels[currentLevel].availableSkills[skill] == 0) return;
		if (!lemmings[index_selected_lem - 1]->isBusy()) {
			if (!lemmings[index_selected_lem - 1]->setSkill(skill)) return;
			else {
				audioDriver.playAudio("sounds/MOUSEPRE.WAV");
				levels[currentLevel].availableSkills[skill]--;
				skillApplied = true;
				previousSkillTime = currentTime;
			}
		}
		else if ((lemmings[index_selected_lem - 1]->isCLimber()) && skill != Lemming::CLIMBER) {
			if (!lemmings[index_selected_lem - 1]->setSkill(skill)) return;
			else {
				audioDriver.playAudio("sounds/MOUSEPRE.WAV");
				levels[currentLevel].availableSkills[skill]--;
				skillApplied = true;
				previousSkillTime = currentTime;
			}
		}
		else if ((lemmings[index_selected_lem - 1]->isFloater()) && skill != Lemming::FLOATER) {
			if (!lemmings[index_selected_lem - 1]->setSkill(skill)) return;
			else {
				audioDriver.playAudio("sounds/MOUSEPRE.WAV");
				levels[currentLevel].availableSkills[skill]--;
				skillApplied = true;
				previousSkillTime = currentTime;
			}
		}
	}
}

void Scene::pressButton()
{
	if (index_selected_but == 0) return;
	if (index_selected_but == 1) checkAndSetReleaseButton();
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
	if (!startSpawn && (float(ct) / 100.f) > 30.f) {
		lemmingTime++;
		previousTime = float(ct) / 100.f;
		startSpawn = true;
	}
	if (startSpawn && previousTime < ((float(ct) / 100.f) - (spawnTime * 10.f))) {
		lemmingTime++;
		previousTime = float(ct) / 100.f;
	}

	return spawnedLemmings < level.lemmingsToSpawn && spawnedLemmings < lemmingTime && !surrStarted;
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
		lem->init(level.spawnPosition, simpleTexProgram, &lemmingTexture, &audioDriver);
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
