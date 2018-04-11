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


void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, Texture * lemmingTexture)
{
	state = FALLING_RIGHT_STATE;
	pending_state = NULL_STATE;
	pending_floater = false;
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.1, 16.0f / SPRITE_HEIGHT), lemmingTexture, &shaderProgram);
	sprite->setNumberAnimations(24);
	
		sprite->setAnimationSpeed(LemmingAnims::WALKING_RIGHT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(LemmingAnims::WALKING_RIGHT, glm::vec2(float(i) / 10, 0.0f));
		
		sprite->setAnimationSpeed(LemmingAnims::WALKING_LEFT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(LemmingAnims::WALKING_LEFT, glm::vec2(float(i) / 10, 16.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::FALLING_RIGHT, 12);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(LemmingAnims::FALLING_RIGHT, glm::vec2(float(i) / 10, 32.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::FALLING_LEFT, 12);
		for (int i = 4; i<8; i++)
			sprite->addKeyframe(LemmingAnims::FALLING_LEFT, glm::vec2(float(i) / 10, 32.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::FLOATER_INI_RIGHT, 10);
		for (int i = 2; i<6; i++)
			sprite->addKeyframe(LemmingAnims::FLOATER_INI_RIGHT, glm::vec2(float(i) / 10, 48.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::FLOATER_INI_LEFT, 10);
		for (int i = 6; i<10; i++)
			sprite->addKeyframe(LemmingAnims::FLOATER_INI_LEFT, glm::vec2(float(i) / 10, 48.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::DIGGING, 10);
		for (int i = 0; i<10; i++)
			sprite->addKeyframe(LemmingAnims::DIGGING, glm::vec2(float(i) / 10, 64.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::FLOATER_RIGHT, 8);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(LemmingAnims::FLOATER_RIGHT, glm::vec2(float(i) / 10, 80.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::FLOATER_LEFT, 8);
		for (int i = 4; i<8; i++)
			sprite->addKeyframe(LemmingAnims::FLOATER_LEFT, glm::vec2(float(i) / 10, 80.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::FALL_DIE, 10);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(LemmingAnims::FALL_DIE, glm::vec2(float(i) / 10, 96.0f / SPRITE_HEIGHT));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(LemmingAnims::FALL_DIE, glm::vec2(float(i) / 10, 112.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::EXPLOSION, 12);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(LemmingAnims::EXPLOSION, glm::vec2(float(i) / 10, 112.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::BLOCKER_ANIM, 12);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(LemmingAnims::BLOCKER_ANIM, glm::vec2(float(i) / 10, 128.0f / SPRITE_HEIGHT));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(LemmingAnims::BLOCKER_ANIM, glm::vec2(float(i) / 10, 144.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::SAFE, 12);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(LemmingAnims::SAFE, glm::vec2(float(i) / 10, 160.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::SURRENDER, 12);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(LemmingAnims::SURRENDER, glm::vec2(float(i) / 10, 176.0f / SPRITE_HEIGHT));
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(LemmingAnims::SURRENDER, glm::vec2(float(i) / 10, 192.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::BASHER_RIGHT, 12);
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BASHER_RIGHT, glm::vec2(float(i) / 10, 320.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BASHER_RIGHT, glm::vec2(float(i) / 10, 336.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BASHER_RIGHT, glm::vec2(float(i) / 10, 352.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BASHER_RIGHT, glm::vec2(float(i) / 10, 368.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::BASHER_LEFT, 12);
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BASHER_LEFT, glm::vec2(float(i) / 10, 384.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BASHER_LEFT, glm::vec2(float(i) / 10, 400.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BASHER_LEFT, glm::vec2(float(i) / 10, 416.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BASHER_LEFT, glm::vec2(float(i) / 10, 432.0f / SPRITE_HEIGHT));
		
	sprite->changeAnimation(LemmingAnims::FALLING_RIGHT);
	sprite->setPosition(initialPosition);

	fallSpeed = 2.0f;
}

void Lemming::update(int deltaTime, int offset, vector<glm::vec2> newColliders)
{
	if (sprite->update(deltaTime) == 0)
		return;

	lem_offset = offset;
	colliders = newColliders;

	switch (state)
	{
	case BLOCKER_LEFT_STATE:
		if (pending_state == SURRENDER_STATE) {
			sprite->changeAnimation(LemmingAnims::SURRENDER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		_block(Lemming::LemmingAnims::FALLING_LEFT, Lemming::LemmingState::FALLING_LEFT_STATE);
		break;

	case BLOCKER_RIGHT_STATE:
		if (pending_state == SURRENDER_STATE) {
			sprite->changeAnimation(LemmingAnims::SURRENDER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		_block(Lemming::LemmingAnims::FALLING_RIGHT, Lemming::LemmingState::FALLING_RIGHT_STATE);
		break;

	case DIGGER_LEFT_STATE:
		if (pending_state == SURRENDER_STATE) {
			sprite->changeAnimation(LemmingAnims::SURRENDER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		_dig(LemmingAnims::FALLING_LEFT, LemmingState::FALLING_LEFT_STATE);
		break;

	case DIGGER_RIGHT_STATE:
		if (pending_state == SURRENDER_STATE) {
			sprite->changeAnimation(LemmingAnims::SURRENDER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		_dig(LemmingAnims::FALLING_RIGHT, LemmingState::FALLING_RIGHT_STATE);
		break;

	case FLOATER_LEFT_STATE:
		if (sprite->animation() == FLOATER_INI_LEFT && sprite->keyFrame() == 3) sprite->changeAnimation(FLOATER_LEFT);
		_float(LemmingAnims::WALKING_LEFT, LemmingState::WALKING_LEFT_STATE);
		break;

	case FLOATER_RIGHT_STATE:		
		if (sprite->animation() == FLOATER_INI_RIGHT && sprite->keyFrame() == 3) sprite->changeAnimation(FLOATER_RIGHT);
		_float(LemmingAnims::WALKING_RIGHT, LemmingState::WALKING_RIGHT_STATE);
		break;

	case FALLING_LEFT_STATE:
		if (pending_floater && sprite->animationTime() > 30.0f) {
			sprite->changeAnimation(LemmingAnims::FLOATER_INI_LEFT);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else _fall(LemmingAnims::WALKING_LEFT, LemmingState::WALKING_LEFT_STATE);
		break;

	case FALLING_RIGHT_STATE:
		if (pending_floater && sprite->animationTime() > 30.0f) {
			sprite->changeAnimation(LemmingAnims::FLOATER_INI_RIGHT);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else _fall(LemmingAnims::WALKING_RIGHT, LemmingState::WALKING_RIGHT_STATE);
		break;

	case WALKING_LEFT_STATE:
		if (pending_state == SURRENDER_STATE) {
			sprite->changeAnimation(LemmingAnims::SURRENDER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (pending_state == DIGGER_LEFT_STATE) {
			sprite->changeAnimation(LemmingAnims::DIGGING);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (pending_state == BLOCKER_LEFT_STATE || pending_state == BLOCKER_RIGHT_STATE) {
			sprite->changeAnimation(LemmingAnims::BLOCKER_ANIM);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else {
			sprite->position() += glm::vec2(-1, -1);
			if (collisionAny()) changeDirection();
			else _walk(LemmingAnims::FALLING_LEFT, LemmingState::FALLING_LEFT_STATE);
		}
		break;

	case WALKING_RIGHT_STATE:
		if (pending_state == SURRENDER_STATE) {
			sprite->changeAnimation(LemmingAnims::SURRENDER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (pending_state == DIGGER_RIGHT_STATE) {
			sprite->changeAnimation(LemmingAnims::DIGGING);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (pending_state == BLOCKER_LEFT_STATE || pending_state == BLOCKER_RIGHT_STATE) {
			sprite->changeAnimation(LemmingAnims::BLOCKER_ANIM);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else {
			sprite->position() += glm::vec2(1, -1);
			if (collisionAny()) changeDirection();
			else _walk(LemmingAnims::FALLING_RIGHT, LemmingState::FALLING_RIGHT_STATE);
		}
		break;
	case SURRENDER_STATE:
		if (sprite->keyFrame() == 15) {
			sprite->changeAnimation(LemmingAnims::EXPLOSION);
			_explote();
			state = EXPLOSION_STATE;
		}
		break;
	case EXPLOSION_STATE:
		if (sprite->keyFrame() == 7) {
			state = DEAD_STATE;
		}
		break;
	case FALL_DIE_STATE:
		if (sprite->keyFrame() == 15) {
			state = DEAD_STATE;
		}
		break;
	case ARRIVE_STATE:
		if (sprite->keyFrame() == 7) {
			state = SAFE_STATE;
		}
		break;
	}
}

bool Lemming::isDead() {
	return state == DEAD_STATE;
}

bool Lemming::isArriving()
{
	return state == ARRIVE_STATE;
}

bool Lemming::isSafe() {
	return state == SAFE_STATE;
}

bool Lemming::isBusy()
{
	return (pending_state != NULL_STATE || pending_floater);
}

bool Lemming::isBlocker()
{
	return (state == BLOCKER_RIGHT_STATE || state == BLOCKER_LEFT_STATE);
}

void Lemming::remove() {
	sprite->free();
}

void Lemming::render()
{
	sprite->render();
}

void Lemming::setMapMask(VariableTexture *mapMask)
{
	mask = mapMask;
}

void Lemming::setPos(glm::vec2 pos)
{
	sprite->setPosition(pos);
}

glm::vec2 Lemming::getPosition() {
	return sprite->position();
}

bool Lemming::setSkill(LemmingSkill newSkill)
{
	if (isSameSkill(newSkill)) return false;

	if (newSkill == FLOATER) {
		pending_floater = true;
	}
	else {
		pending_state = getStateFromSkill(newSkill);
		pending_floater = false;
	}
	return true;
}

void Lemming::lemmingArrived()
{
	sprite->changeAnimation(LemmingAnims::SAFE);
	state = ARRIVE_STATE;
}

Lemming::LemmingState Lemming::getStateFromSkill(LemmingSkill skill) 
{
	if (skill == SURREND) return SURRENDER_STATE;
	if (isGoingLeft()) {
		if (skill == BLOCKER) return BLOCKER_LEFT_STATE;
		if (skill == DIGGER) return DIGGER_LEFT_STATE;
		if (skill == FLOATER) return FLOATER_LEFT_STATE;
	}
	else {
		if (skill == BLOCKER) return BLOCKER_RIGHT_STATE;
		if (skill == DIGGER) return DIGGER_RIGHT_STATE;
		if (skill == FLOATER) return FLOATER_RIGHT_STATE;
	}
}

bool Lemming::isGoingLeft() {
	return (state == WALKING_LEFT_STATE || state == FALLING_LEFT_STATE || state == DIGGER_LEFT_STATE || state == FLOATER_LEFT_STATE);
}
bool Lemming::isGoingRight() {
	return !isGoingLeft();
}

bool Lemming::isSameSkill(LemmingSkill newSkill) 
{
	if (newSkill == BLOCKER && (state == BLOCKER_RIGHT_STATE || state == BLOCKER_LEFT_STATE)) return true;
	if (newSkill == DIGGER && (state == DIGGER_RIGHT_STATE || state == DIGGER_LEFT_STATE)) return true;
	if (newSkill == FLOATER && (state == FLOATER_RIGHT_STATE || state == FLOATER_LEFT_STATE)) return true;
	return false;
}


void Lemming::_block(LemmingAnims fallAnim, LemmingState fallState) {
	int fall = collisionFloor(3);
	if (fall > 0) {
		sprite->changeAnimation(fallAnim);
		state = fallState;
	}
}

void Lemming::_dig(LemmingAnims fallAnim, LemmingState fallState)
{
	int posX, posY;
	int fall = collisionAny(3);
	if (fall > 0) {
		sprite->position() += glm::vec2(0, 1);
	}
	if (fall > 1) {
		sprite->changeAnimation(fallAnim);
		state = fallState;
	}
	else if (sprite->keyFrame() % 5 == 0) {
		sprite->position() += glm::vec2(0, 1);

		posX = int(sprite->position().x) + lem_offset + 7;
		posY = int(sprite->position().y) + 15;
		for (int y = max(0, posY); y <= min(mask->height() - 1, posY); y++)
			for (int x = max(0, posX - 4); x <= min(mask->width() - 1, posX + 5); x++)
				mask->setPixel(x, y, 0);
	}

}


void Lemming::_walk(LemmingAnims fallAnimation, LemmingState fallState)
{
	int fall = collisionAny(3);
	if (fall > 0)
		sprite->position() += glm::vec2(0, 1);
	if (fall > 1)
		sprite->position() += glm::vec2(0, 1);
	if (fall > 2) {
		sprite->changeAnimation(fallAnimation);
		state = fallState;
	}
}


void Lemming::_float(LemmingAnims walkAnimation, LemmingState walkState)
{
	int fall = collisionFloor(2);
	if (fall > 0)
		sprite->position() += glm::vec2(0, fall);
	else {
		sprite->changeAnimation(walkAnimation);
		state = walkState;
	}
}

void Lemming::_fall(LemmingAnims walkAnimation, LemmingState walkState) 
{
	int fall = collisionFloor(3);
	if (fall == 3) {
		sprite->position() += glm::vec2(0, fallSpeed);
		incrementFallSpeed();
	}
	else if (fall == 0) { 
		if (fallSpeed == 3.0f) {
			sprite->changeAnimation(LemmingAnims::FALL_DIE);
			state = FALL_DIE_STATE;
		}
		else {
			fallSpeed = MINIMUM_FALL_SPEED;
			sprite->changeAnimation(walkAnimation);
			state = walkState;
		}
	}
	else {
		sprite->position() += glm::vec2(0, fall);
	}
}

void Lemming::_explote()
{
	int posX, posY;

	posX = sprite->position().x + lem_offset + 7;
	posY = sprite->position().y + 12;
	int r = 9;
	int dx;
	int dy;
	for (int y = max(0, posY - r); y <= min(mask->height() - 1, posY + r); y++) {
		for (int x = max(0, posX - r); x <= min(mask->width() - 1, posX + r); x++) {
			dx = posX - x; // horizontal offset
			dy = posY - y; // vertical offset
			if ((dx*dx + dy * dy) <= (r*r))
			{
				mask->setPixel(x, y, 0);
			}
		}
	}
}

int Lemming::collisionAny(int maxFall) {
	return min(collisionFloor(maxFall), collisionCollider(maxFall));
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(lem_offset, 0); // Add the map displacement
	
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

int Lemming::collisionCollider(int maxFall) {
	int minFall = maxFall;

	for (int i = 0; i < colliders.size() && minFall > 0; i += 2) {
		glm::vec2 colliderStart = colliders[i];
		glm::vec2 colliderEnd = colliders[i+1];

		bool bContact = false;
		int fall = 0;
		glm::vec2 position = sprite->position() + glm::vec2(lem_offset, 0); // Add the map displacement
		while ((fall < maxFall) && !bContact)
		{
			glm::vec2 lemmingStart = position + glm::vec2(7, 15 + fall);
			glm::vec2 lemmingEnd = position + glm::vec2(8, 15 + fall);
			if (collideWithBlocker(lemmingStart, lemmingEnd, colliderStart, colliderEnd)) bContact = true;
			else ++fall;
		}
		minFall = min(fall, minFall);
	}
	return minFall;
}

bool Lemming::collisionAny() {
	return collisionFloor() || collisionCollider();
}

bool Lemming::collisionFloor()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(lem_offset, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}

bool Lemming::collisionCollider() {
	for (int i = 0; i < colliders.size(); i += 2) {
		glm::vec2 colliderStart = colliders[i];
		glm::vec2 colliderEnd = colliders[i + 1];

		glm::vec2 position = sprite->position() + glm::vec2(lem_offset, 0); // Add the map displacement

		glm::vec2 lemmingStart = position + glm::vec2(7, 0);
		glm::vec2 lemmingEnd = position + glm::vec2(8, 15);

		if (collideWithBlocker(lemmingStart, lemmingEnd, colliderStart, colliderEnd)) return true;;
	}
	return false;
}

bool Lemming::collideWithBlocker(glm::vec2 startBox1, glm::vec2 endBox1, glm::vec2 startBox2, glm::vec2 endBox2)
{
	if ((endBox1.y >= startBox2.y && endBox1.y <= endBox2.y) || (startBox1.y >= startBox2.y && startBox1.y <= endBox2.y) ||
		(startBox1.y >= startBox2.y && endBox1.y <= endBox2.y) || (startBox2.y > startBox1.y && endBox2.y < endBox1.y)) {
		if (state == WALKING_LEFT_STATE && startBox1.x == endBox2.x) return true;
		if (state == WALKING_RIGHT_STATE && endBox1.x == startBox2.x) return true;
	}
	return false;
}

void Lemming::changeDirection() {
	if (state == WALKING_LEFT_STATE) {
		sprite->position() -= glm::vec2(-1, -1);
		sprite->changeAnimation(LemmingAnims::WALKING_RIGHT);
		state = WALKING_RIGHT_STATE;
	}
	else {
		sprite->position() -= glm::vec2(1, -1);
		sprite->changeAnimation(LemmingAnims::WALKING_LEFT);
		state = WALKING_LEFT_STATE;
	}
}

void Lemming::incrementFallSpeed() {
	if (fallSpeed < MAXIMUM_FALL_SPEED)  fallSpeed += GRAVITY;
	else fallSpeed = MAXIMUM_FALL_SPEED;
}



