#include <iostream>
#include "Countdown.h"



void Countdown::initCountdown(glm::vec2 lemPosition, ShaderProgram &shaderProgram, Texture * lemmingTexture)
{	
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.1, 16.0f / SPRITE_HEIGHT), lemmingTexture, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(ON, 1);
	for (int i = 8; i<10; i++)
		sprite->addKeyframe(ON, glm::vec2(float(i) / 10, 0.0f));
	for (int i = 8; i<10; i++)
		sprite->addKeyframe(ON, glm::vec2(float(i) / 10, 16.0f / SPRITE_HEIGHT));
	for (int i = 8; i<10; i++)
		sprite->addKeyframe(ON, glm::vec2(float(i) / 10, 32.0f / SPRITE_HEIGHT));
	sprite->setAnimationSpeed(OFF, 1);
	sprite->addKeyframe(OFF, glm::vec2(float(9) / 10, 32.0f / SPRITE_HEIGHT));

	sprite->changeAnimation(OFF);
	sprite->setPosition(glm::vec2(lemPosition.x, lemPosition.y - 16));
}

bool Countdown::update(glm::vec2 lemPosition, int deltaTime)
{
	bool finished = false;
	sprite->setPosition(glm::vec2(lemPosition.x, lemPosition.y - 16));
	if (sprite->update(deltaTime) == 0)
		return false;
	switch (state)
	{
	case ON:
		if (sprite->keyFrame() == 5) {
			state = OFF;
			sprite->changeAnimation(OFF);
			finished = true;
		}
		break;
	}
	return finished;
}

void Countdown::render()
{
	sprite->render();
}

void Countdown::remove() {
	sprite->free();
}

void Countdown::setOn()
{
	state = ON;
	sprite->changeAnimation(ON);
}
