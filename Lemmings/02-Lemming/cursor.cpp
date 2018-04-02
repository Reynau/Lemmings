#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "cursor.h"



void Cursor::initCursor(ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/lemming.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite_h = 160.0f;
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.1, 16.0f / sprite_h), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

		sprite->setAnimationSpeed(NORMAL, 12);	// NORMAL
		sprite->addKeyframe(NORMAL, glm::vec2(float(1) / 10, 48.0f / sprite_h));

		sprite->setAnimationSpeed(SELECT, 12);	// SELECTED
		sprite->addKeyframe(SELECT, glm::vec2(float(0) / 10, 48.0f / sprite_h));

	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(0, 0));
}

glm::vec2 Cursor::getPos()
{
	return sprite->position();
}

void Cursor::setPos(int mouseX, int mouseY)
{
	sprite->setPosition(glm::vec2((mouseX / 3) - 7, (mouseY / 3) - 7));
}

void Cursor::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
}

void Cursor::render()
{
	sprite->render();
}

void Cursor::setSelect(bool selecting)
{
	if (selecting && sprite->animation() == NORMAL) sprite->changeAnimation(SELECT);
	else if (!selecting && sprite->animation() == SELECT) sprite->changeAnimation(NORMAL);
}

