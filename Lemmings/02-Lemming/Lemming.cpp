#include <cmath>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum LemmingAnims
{
	WALKING_LEFT, WALKING_RIGHT, FALLING_LEFT, FALLING_RIGHT, DIGGING, FLOATER_INI_RIGHT, FLOATER_INI_LEFT, FLOATER_RIGHT, FLOATER_LEFT, FALL_DIE, BLOCKER
};


void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, Texture * lemmingTexture)
{
	state = FALLING_RIGHT_STATE;
	pending_state = NULL_STATE;
	pending_floater = false;
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.1, 16.0f / SPRITE_HEIGHT), lemmingTexture, &shaderProgram);
	sprite->setNumberAnimations(11);
	
		sprite->setAnimationSpeed(WALKING_RIGHT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / 10, 0.0f));
		
		sprite->setAnimationSpeed(WALKING_LEFT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_LEFT, glm::vec2(float(i) / 10, 16.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(FALLING_RIGHT, 12);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(FALLING_RIGHT, glm::vec2(float(i) / 10, 32.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(FALLING_LEFT, 12);
		for (int i = 4; i<8; i++)
			sprite->addKeyframe(FALLING_LEFT, glm::vec2(float(i) / 10, 32.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(DIGGING, 10);
		for (int i = 0; i<10; i++)
			sprite->addKeyframe(DIGGING, glm::vec2(float(i) / 10, 64.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(FLOATER_INI_RIGHT, 10);
		for (int i = 2; i<6; i++)
			sprite->addKeyframe(FLOATER_INI_RIGHT, glm::vec2(float(i) / 10, 48.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(FLOATER_INI_LEFT, 10);
		for (int i = 6; i<10; i++)
			sprite->addKeyframe(FLOATER_INI_LEFT, glm::vec2(float(i) / 10, 48.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(FLOATER_RIGHT, 8);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(FLOATER_RIGHT, glm::vec2(float(i) / 10, 80.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(FLOATER_LEFT, 8);
		for (int i = 4; i<8; i++)
			sprite->addKeyframe(FLOATER_LEFT, glm::vec2(float(i) / 10, 80.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(FALL_DIE, 10);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(FALL_DIE, glm::vec2(float(i) / 10, 96.0f / SPRITE_HEIGHT));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(FALL_DIE, glm::vec2(float(i) / 10, 112.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(BLOCKER, 12);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BLOCKER, glm::vec2(float(i) / 10, 128.0f / SPRITE_HEIGHT));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(BLOCKER, glm::vec2(float(i) / 10, 144.0f / SPRITE_HEIGHT));
		
	sprite->changeAnimation(FALLING_RIGHT);
	sprite->setPosition(initialPosition);

	fallSpeed = 2.0f;
}

void Lemming::update(int deltaTime)
{
	int fall;
	//int posX, posY;

	if (sprite->update(deltaTime) == 0)
		return;

	switch (state)
	{
	case BLOCKER_STATE:
		/*posX = (sprite->position()).x + 127;
		posY = (sprite->position()).y + 7;
		for (int y = max(0, posY - 8); y <= min(mask->height() - 1, posY + 8); y++)
			for (int x = max(0, posX - 5); x <= min(mask->width() - 1, posX -5); x++)
				mask->setPixel(x, y, 255);
		for (int y = max(0, posY - 8); y <= min(mask->height() - 1, posY + 8); y++)
			for (int x = max(0, posX + 5); x <= min(mask->width() - 1, posX + 5); x++)
				mask->setPixel(x, y, 255);*/
		break;
	case DIGGER_LEFT_STATE:
		dig();

		fall = collisionFloor(3);
		if (fall > 0) {
			sprite->position() += glm::vec2(0, 1);
		}
		if (fall > 1) {
			sprite->changeAnimation(FALLING_LEFT);
			state = FALLING_LEFT_STATE;
		}
		break;
	case DIGGER_RIGHT_STATE:
		dig();

		fall = collisionFloor(3);
		if (fall > 0) {
			sprite->position() += glm::vec2(0, 1);
		}
		if (fall > 1) {
			sprite->changeAnimation(FALLING_RIGHT);
			state = FALLING_RIGHT_STATE;
		}
		break;
	case FLOATER_LEFT_STATE:
		if (sprite->animation() == FLOATER_INI_LEFT && sprite->keyFrame() == 3) sprite->changeAnimation(FLOATER_LEFT);
		fall = collisionFloor(2);
		if (fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else {
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		break;
	case FLOATER_RIGHT_STATE:		
		if (sprite->animation() == FLOATER_INI_RIGHT && sprite->keyFrame() == 3) sprite->changeAnimation(FLOATER_RIGHT);
		fall = collisionFloor(2);
		if (fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else {
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		break;
	case FALLING_LEFT_STATE:
		if (pending_floater && sprite->animationTime() > 30.0f) {
			sprite->changeAnimation(FLOATER_INI_LEFT);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else {
			fall = collisionFloor(3);
			if (fall > 1) {	// If is not going to touch the floor
				sprite->position() += glm::vec2(0, fallSpeed);
				incrementFallSpeed();
			}
			else {			// If is going to touch the floor
				if (fallSpeed >= 3.0f) {
					sprite->changeAnimation(FALL_DIE);
					state = FALL_DIE_STATE;
				}
				else {
					fallSpeed = MINIMUM_FALL_SPEED;
					sprite->changeAnimation(WALKING_LEFT);
					state = WALKING_LEFT_STATE;
				}
			}
		}
		break;
	case FALLING_RIGHT_STATE:
		if (pending_floater && sprite->animationTime() > 30.0f) {
			sprite->changeAnimation(FLOATER_INI_RIGHT);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else {
			fall = collisionFloor(3);
			if (fall > 1) {
				sprite->position() += glm::vec2(0, fallSpeed);
				incrementFallSpeed();
				cout << fallSpeed << endl;
			}
			else {
				if (fallSpeed == 3.0f) {
					sprite->changeAnimation(FALL_DIE);
					state = FALL_DIE_STATE;
				}
				else {
					fallSpeed = MINIMUM_FALL_SPEED;
					sprite->changeAnimation(WALKING_RIGHT);
					state = WALKING_RIGHT_STATE;
				}
			}
		}
		break;
	case WALKING_LEFT_STATE:
		if (pending_state == DIGGER_LEFT_STATE) {
			sprite->changeAnimation(DIGGING);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (pending_state == BLOCKER_STATE) {
			sprite->changeAnimation(BLOCKER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else {
			sprite->position() += glm::vec2(-1, -1);
			if (collision())
			{
				sprite->position() -= glm::vec2(-1, -1);
				sprite->changeAnimation(WALKING_RIGHT);
				state = WALKING_RIGHT_STATE;
			}
			else
			{
				fall = collisionFloor(3);
				if (fall > 0)
					sprite->position() += glm::vec2(0, 1);
				if (fall > 1)
					sprite->position() += glm::vec2(0, 1);
				if (fall > 2) {
					sprite->changeAnimation(FALLING_LEFT);
					state = FALLING_LEFT_STATE;
				}
			}
		}
		break;
	case WALKING_RIGHT_STATE:
		if (pending_state == DIGGER_RIGHT_STATE) {
			sprite->changeAnimation(DIGGING);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (pending_state == BLOCKER_STATE) {
			sprite->changeAnimation(BLOCKER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else {
			sprite->position() += glm::vec2(1, -1);
			if (collision())
			{
				sprite->position() -= glm::vec2(1, -1);
				sprite->changeAnimation(WALKING_LEFT);
				state = WALKING_LEFT_STATE;
			}
			else
			{
				fall = collisionFloor(3);
				if (fall > 0)
					sprite->position() += glm::vec2(0, 1);
				if (fall > 1)
					sprite->position() += glm::vec2(0, 1);
				if (fall > 2) {
					sprite->changeAnimation(FALLING_RIGHT);
					state = FALLING_RIGHT_STATE;
				}
			}
		}
		break;
	case FALL_DIE_STATE:
		if (sprite->keyFrame() == 15) {
			state = DEAD_STATE;
		}
		break;
	}
}

bool Lemming::isDead() {
	return state == DEAD_STATE;
}

void Lemming::remove() {
	sprite->free();
}

bool Lemming::occupied()
{
	return (pending_state != NULL_STATE || pending_floater);
}

void Lemming::render()
{
	sprite->render();
}

void Lemming::setMapMask(VariableTexture *mapMask)
{
	mask = mapMask;
}

glm::vec2 Lemming::getPosition() {
	return sprite->position();
}


bool Lemming::setState(LemmingState selected_state)
{
	if ((selected_state == FLOATER_LEFT_STATE || selected_state == FLOATER_RIGHT_STATE) && (state != FLOATER_LEFT_STATE || state != FLOATER_RIGHT_STATE)) {
		pending_floater = true;
		return true;
	}
	else if (selected_state != state) {
		pending_state = selected_state;
		pending_floater = false;
		return true;
	}
	else
		return false;
}



void Lemming::dig() {
	int posX, posY;
	if (sprite->keyFrame() % 5 == 0) {
		sprite->position() += glm::vec2(0, 1);

		posX = int(sprite->position().x) + 127;
		posY = int(sprite->position().y) + 15;
		for (int y = max(0, posY); y <= min(mask->height() - 1, posY); y++)
			for (int x = max(0, posX - 3); x <= min(mask->width() - 1, posX + 4); x++)
				mask->setPixel(x, y, 0);
	}
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 16);
	while((fall < maxFall) && !bContact)
	{
		if((mask->pixel(posBase.x, posBase.y+fall) == 0) && (mask->pixel(posBase.x+1, posBase.y+fall) == 0))
			fall += 1;
		else
			bContact = true;
	}

	return fall;
}

bool Lemming::collision()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}

void Lemming::incrementFallSpeed() {
	if (fallSpeed < MAXIMUM_FALL_SPEED)  fallSpeed += GRAVITY;
	else fallSpeed = MAXIMUM_FALL_SPEED;
}





