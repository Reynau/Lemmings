#include <GL/glew.h>
#include "Button.h"



void Button::initButton(ShaderProgram &shaderProgram, Texture * buttonTexture)
{
	sprite = Sprite::createSprite(glm::ivec2(26, 30), glm::vec2(0.5, 1.f), buttonTexture, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(NORMAL, 12);	// NORMAL
	sprite->addKeyframe(NORMAL, glm::vec2(0.f, 1.f));

	sprite->setAnimationSpeed(SELECT, 12);	// SELECTED
	sprite->addKeyframe(SELECT, glm::vec2(0.5f, 1.f));

	sprite->changeAnimation(0);
	sprite->setPosition(glm::vec2(0, 0));
}

glm::vec2 Button::getPos()
{
	return sprite->position();
}

void Button::setPos(float x, float y)
{
	sprite->setPosition(glm::vec2(x, y));
}

void Button::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
}

void Button::render()
{
	sprite->render();
}

void Button::setSelect(bool selecting)
{
	if (selecting && sprite->animation() == NORMAL) sprite->changeAnimation(SELECT);
	else if (!selecting && sprite->animation() == SELECT) sprite->changeAnimation(NORMAL);
}