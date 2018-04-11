#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "cursor.h"



void Cursor::initCursor(ShaderProgram &shaderProgram, Texture * lemmingTexture)
{
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.1, 16.0f / SPRITE_HEIGHT), lemmingTexture, &shaderProgram);
	sprite->setNumberAnimations(2);

		sprite->setAnimationSpeed(NORMAL, 12);	// NORMAL
		sprite->addKeyframe(NORMAL, glm::vec2(float(1) / 10, 48.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(SELECT, 12);	// SELECTED
		sprite->addKeyframe(SELECT, glm::vec2(float(0) / 10, 48.0f / SPRITE_HEIGHT));

	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(0, 0));
}

glm::vec2 Cursor::getPos()
{
	return sprite->position();
}

void Cursor::setPos(int mouseX, int mouseY) 
{
	sprite->setPosition(glm::vec2((mouseX * ((320.f - 1.f)/(glutGet(GLUT_WINDOW_WIDTH)))) - 8, (mouseY * ((160.f - 1.f)/(glutGet(GLUT_WINDOW_HEIGHT) - (40.f * ((glutGet(GLUT_WINDOW_HEIGHT) - 190.f) / 160.f))))) - 8));
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

