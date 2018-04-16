#include <GL/glew.h>
#include "Button.h"



void Button::initButton(ShaderProgram &shaderProgram, Texture * buttonTexture)
{
	sprite = Sprite::createSprite(glm::ivec2(26, 30), glm::vec2(1.f/11.f, 30.0f/SPRITE_HEIGHT), buttonTexture, &shaderProgram);
	sprite->setNumberAnimations(23);

	sprite->setAnimationSpeed(NORMAL_DIGGER, 1);
	sprite->addKeyframe(NORMAL_DIGGER, glm::vec2(0.f, 0.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(SELECT_DIGGER, 10);
	for (int i = 1; i<11; i++)
		sprite->addKeyframe(SELECT_DIGGER, glm::vec2(float(i) / 11.f, 0.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(NORMAL_FLOATER, 1);
	sprite->addKeyframe(NORMAL_FLOATER, glm::vec2(0.f, 30.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(SELECT_FLOATER, 8);
	for (int i = 1; i<5; i++)
		sprite->addKeyframe(SELECT_FLOATER, glm::vec2(float(i) / 11.f, 30.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(INCREASE_DECREASE_RATING, 1);
	sprite->addKeyframe(INCREASE_DECREASE_RATING, glm::vec2(5.f / 11.f, 30.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(NORMAL_CLIMBER, 1);
	sprite->addKeyframe(NORMAL_CLIMBER, glm::vec2(0.f, 60.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(SELECT_CLIMBER, 12);
	for (int i = 1; i<9; i++)
		sprite->addKeyframe(SELECT_CLIMBER, glm::vec2(float(i) / 11.f, 60.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(NORMAL_EXPLOSIVE, 1);
	sprite->addKeyframe(NORMAL_EXPLOSIVE, glm::vec2(0.f, 90.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(SELECT_EXPLOSIVE, 6);
	for (int i = 1; i<9; i++)
		sprite->addKeyframe(SELECT_EXPLOSIVE, glm::vec2(float(i) / 11.f, 90.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(NORMAL_BLOCKER, 1);
	sprite->addKeyframe(NORMAL_BLOCKER, glm::vec2(0.f, 120.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(SELECT_BLOCKER, 12);
	for (int i = 1; i<11; i++)
		sprite->addKeyframe(SELECT_BLOCKER, glm::vec2(float(i) / 11.f, 120.f / SPRITE_HEIGHT));
	for (int i = 6; i<11; i++)
		sprite->addKeyframe(SELECT_BLOCKER, glm::vec2(float(i) / 11.f, 30.f / SPRITE_HEIGHT));
	sprite->addKeyframe(SELECT_BLOCKER, glm::vec2(9.f / 11.f, 60.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(NORMAL_BUILDER, 1);
	sprite->addKeyframe(NORMAL_BUILDER, glm::vec2(0.f, 150.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(SELECT_BUILDER, 12);
	for (int i = 1; i<11; i++)
		sprite->addKeyframe(SELECT_BUILDER, glm::vec2(float(i) / 11.f, 150.f / SPRITE_HEIGHT));
	for (int i = 5; i<11; i++)
		sprite->addKeyframe(SELECT_BUILDER, glm::vec2(float(i) / 11.f, 180.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(NORMAL_BASHER, 1);
	sprite->addKeyframe(NORMAL_BASHER, glm::vec2(0.f, 180.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(SELECT_BASHER, 12);
	for (int i = 1; i<5; i++)
		sprite->addKeyframe(SELECT_BASHER, glm::vec2(float(i) / 11.f, 180.f / SPRITE_HEIGHT));
	for (int i = 1; i<11; i++)
		sprite->addKeyframe(SELECT_BASHER, glm::vec2(float(i) / 11.f, 210.f / SPRITE_HEIGHT));
	for (int i = 1; i<11; i++)
		sprite->addKeyframe(SELECT_BASHER, glm::vec2(float(i) / 11.f, 240.f / SPRITE_HEIGHT));
	for (int i = 1; i<5; i++)
		sprite->addKeyframe(SELECT_BASHER, glm::vec2(float(i) / 11.f, 270.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(NORMAL_DIAG_BASHER, 1);
	sprite->addKeyframe(NORMAL_DIAG_BASHER, glm::vec2(5.f / 11.f, 270.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(SELECT_DIAG_BASHER, 12);
	for (int i = 6; i<11; i++)
		sprite->addKeyframe(SELECT_DIAG_BASHER, glm::vec2(float(i) / 11.f, 270.f / SPRITE_HEIGHT));
	for (int i = 1; i<11; i++)
		sprite->addKeyframe(SELECT_DIAG_BASHER, glm::vec2(float(i) / 11.f, 300.f / SPRITE_HEIGHT));
	for (int i = 1; i<8; i++)
		sprite->addKeyframe(SELECT_DIAG_BASHER, glm::vec2(float(i) / 11.f, 330.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(NORMAL_PAUSE, 1);
	sprite->addKeyframe(NORMAL_PAUSE, glm::vec2(0.f, 360.f / SPRITE_HEIGHT));
	sprite->setAnimationSpeed(SELECT_PAUSE, 1);
	sprite->addKeyframe(SELECT_PAUSE, glm::vec2(1.f / 11.f, 360.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(NORMAL_SURREND, 1);
	sprite->addKeyframe(NORMAL_SURREND, glm::vec2(2.f / 11.f, 360.f / SPRITE_HEIGHT));
	sprite->setAnimationSpeed(SELECT_SURREND, 1);
	sprite->addKeyframe(SELECT_SURREND, glm::vec2(3.f / 11.f, 360.f / SPRITE_HEIGHT));

	sprite->setAnimationSpeed(NORMAL_FAST, 1);
	sprite->addKeyframe(NORMAL_FAST, glm::vec2(4.f / 11.f, 360.f / SPRITE_HEIGHT));
	sprite->setAnimationSpeed(SELECT_FAST, 2);
	for (int i = 5; i<9; i++)
		sprite->addKeyframe(SELECT_FAST, glm::vec2(float(i) / 11.f, 360.f / SPRITE_HEIGHT));

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

bool Button::isSelect()
{
	return (sprite->animation() == SELECT_DIGGER ||
			sprite->animation() == SELECT_FLOATER ||
			sprite->animation() == SELECT_CLIMBER ||
			sprite->animation() == SELECT_EXPLOSIVE ||
			sprite->animation() == SELECT_BLOCKER ||
			sprite->animation() == SELECT_BUILDER ||
			sprite->animation() == SELECT_BASHER ||
			sprite->animation() == SELECT_DIAG_BASHER ||
			sprite->animation() == SELECT_PAUSE || 
			sprite->animation() == SELECT_SURREND || 
			sprite->animation() == SELECT_FAST);
}

int Button::getAnim()
{
	return sprite->animation();
}

void Button::setAnim(buttonAnim anim)
{
	sprite->changeAnimation(anim);
}

void Button::setSelect(bool selection)
{
	if (selection) {
		if (sprite->animation() == NORMAL_DIGGER) sprite->changeAnimation(SELECT_DIGGER);
		if (sprite->animation() == NORMAL_FLOATER) sprite->changeAnimation(SELECT_FLOATER);
		if (sprite->animation() == NORMAL_CLIMBER) sprite->changeAnimation(SELECT_CLIMBER);
		if (sprite->animation() == NORMAL_EXPLOSIVE) sprite->changeAnimation(SELECT_EXPLOSIVE);
		if (sprite->animation() == NORMAL_BLOCKER) sprite->changeAnimation(SELECT_BLOCKER);
		if (sprite->animation() == NORMAL_BUILDER) sprite->changeAnimation(SELECT_BUILDER);
		if (sprite->animation() == NORMAL_BASHER) sprite->changeAnimation(SELECT_BASHER);
		if (sprite->animation() == NORMAL_DIAG_BASHER) sprite->changeAnimation(SELECT_DIAG_BASHER);
		if (sprite->animation() == NORMAL_PAUSE) sprite->changeAnimation(SELECT_PAUSE);
		if (sprite->animation() == NORMAL_SURREND) sprite->changeAnimation(SELECT_SURREND);
		if (sprite->animation() == NORMAL_FAST) sprite->changeAnimation(SELECT_FAST);
	}
	else {
		if (sprite->animation() == SELECT_DIGGER) sprite->changeAnimation(NORMAL_DIGGER);
		if (sprite->animation() == SELECT_FLOATER) sprite->changeAnimation(NORMAL_FLOATER);
		if (sprite->animation() == SELECT_CLIMBER) sprite->changeAnimation(NORMAL_CLIMBER);
		if (sprite->animation() == SELECT_EXPLOSIVE) sprite->changeAnimation(NORMAL_EXPLOSIVE);
		if (sprite->animation() == SELECT_BLOCKER) sprite->changeAnimation(NORMAL_BLOCKER);
		if (sprite->animation() == SELECT_BUILDER) sprite->changeAnimation(NORMAL_BUILDER);
		if (sprite->animation() == SELECT_BASHER) sprite->changeAnimation(NORMAL_BASHER);
		if (sprite->animation() == SELECT_DIAG_BASHER) sprite->changeAnimation(NORMAL_DIAG_BASHER);
		if (sprite->animation() == SELECT_PAUSE) sprite->changeAnimation(NORMAL_PAUSE);
		if (sprite->animation() == SELECT_SURREND) sprite->changeAnimation(NORMAL_SURREND);
		if (sprite->animation() == SELECT_FAST) sprite->changeAnimation(NORMAL_FAST);
	}
	
}