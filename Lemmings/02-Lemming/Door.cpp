#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Door.h"

void Door::initDoor(ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/doors.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(0.125, 48.0f / SPRITE_HEIGHT), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

		sprite->setAnimationSpeed(FIRST_DOOR, 12);
		for (int i = 0; i<6; i++)
			sprite->addKeyframe(FIRST_DOOR, glm::vec2(float(i) / 8, 0));

		sprite->setAnimationSpeed(SECOND_DOOR, 12);
		for (int i = 0; i<6; i++)
			sprite->addKeyframe(SECOND_DOOR, glm::vec2(float(i) / 8, 48.0f / SPRITE_HEIGHT));

	sprite->changeAnimation(FIRST_DOOR);
	sprite->setPosition(glm::vec2(0, 0));
}

void Door::setPos(glm::vec2 pos)
{
	sprite->setPosition(pos);
}

void Door::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
}

void Door::render()
{
	sprite->render();
}

void Door::setState(doorState door_state)
{
	if (sprite->animation() != door_state) sprite->changeAnimation(door_state);
}
