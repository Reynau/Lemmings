#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Door.h"

void Door::initDoor(ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/doors.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.1, 48.0f / SPRITE_HEIGHT), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);

		sprite->setAnimationSpeed(FIRST_DOOR, 12);
		for (int i = 0; i<6; i++)
			sprite->addKeyframe(FIRST_DOOR, glm::vec2(float(i) / 10, 0));

		sprite->setAnimationSpeed(SECOND_DOOR, 12);
		for (int i = 0; i<6; i++)
			sprite->addKeyframe(SECOND_DOOR, glm::vec2(float(i) / 10, 48.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(THIRD_DOOR, 12);
		for (int i = 0; i<6; i++)
			sprite->addKeyframe(THIRD_DOOR, glm::vec2(float(i) / 10, 144.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(SPAWN_DOOR_CLOSE, 12);
		sprite->addKeyframe(SPAWN_DOOR_CLOSE, glm::vec2(float(0) / 10, 96.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(SPAWN_DOOR, 12);
		for (int i = 0; i<10; i++)
			sprite->addKeyframe(SPAWN_DOOR, glm::vec2(float(i) / 10, 96.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(SPAWN_DOOR_OPEN, 12);
		sprite->addKeyframe(SPAWN_DOOR_OPEN, glm::vec2(float(9) / 10, 96.0f / SPRITE_HEIGHT));

	sprite->changeAnimation(FIRST_DOOR);
	sprite->setPosition(glm::vec2(0, 0));
}

void Door::setPos(glm::vec2 pos)
{
	sprite->setPosition(pos);
}

void Door::update(int deltaTime, int currentTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
	if (sprite->animation() == SPAWN_DOOR_CLOSE && currentTime > 1) sprite->changeAnimation(SPAWN_DOOR);
	else if (sprite->animation() == SPAWN_DOOR && sprite->keyFrame() == 9) sprite->changeAnimation(SPAWN_DOOR_OPEN);
}

void Door::render()
{
	sprite->render();
}

void Door::setState(doorState door_state)
{
	if (sprite->animation() != door_state) sprite->changeAnimation(door_state);
}
