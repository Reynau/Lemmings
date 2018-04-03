#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Door.h"

void Door::initDoor(ShaderProgram & shaderProgram)
{
	spritesheet.loadFromFile("images/doors.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite_h = 48.0f;
	sprite = Sprite::createSprite(glm::ivec2(48, 48), glm::vec2(1, 48.0f / sprite_h), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(FIRST_DOOR, 12);
	sprite->addKeyframe(FIRST_DOOR, glm::vec2(float(0) / 1, 48.0f / sprite_h));

	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(0, 0));
}

void Door::setPos(glm::vec2 pos)
{
	sprite->setPosition(pos);
}

void Door::render()
{
	sprite->render();
}
