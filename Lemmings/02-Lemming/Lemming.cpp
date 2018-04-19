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
	pendingFloater = false;
	isBashing = false;
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.1, 16.0f / SPRITE_HEIGHT), lemmingTexture, &shaderProgram);
	sprite->setNumberAnimations(30);
	
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

		sprite->setAnimationSpeed(LemmingAnims::DIAG_BASHER_RIGHT, 12);
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::DIAG_BASHER_RIGHT, glm::vec2(float(i) / 10, 208.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::DIAG_BASHER_RIGHT, glm::vec2(float(i) / 10, 224.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::DIAG_BASHER_RIGHT, glm::vec2(float(i) / 10, 240.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::DIAG_BASHER_LEFT, 12);
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::DIAG_BASHER_LEFT, glm::vec2(float(i) / 10, 256.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::DIAG_BASHER_LEFT, glm::vec2(float(i) / 10, 272.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::DIAG_BASHER_LEFT, glm::vec2(float(i) / 10, 288.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)

		sprite->setAnimationSpeed(LemmingAnims::BASHER_RIGHT, 12);
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BASHER_RIGHT, glm::vec2(float(i) / 10, 304.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BASHER_RIGHT, glm::vec2(float(i) / 10, 320.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BASHER_RIGHT, glm::vec2(float(i) / 10, 336.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BASHER_RIGHT, glm::vec2(float(i) / 10, 352.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::BASHER_LEFT, 12);
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BASHER_LEFT, glm::vec2(float(i) / 10, 368.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BASHER_LEFT, glm::vec2(float(i) / 10, 384.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BASHER_LEFT, glm::vec2(float(i) / 10, 400.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BASHER_LEFT, glm::vec2(float(i) / 10, 416.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::CLIMBER_RIGHT, 12);
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::CLIMBER_RIGHT, glm::vec2(float(i) / 10, 432.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::CLIMBER_UP_RIGHT, 12);
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::CLIMBER_UP_RIGHT, glm::vec2(float(i) / 10, 448.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::CLIMBER_LEFT, 12);
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::CLIMBER_LEFT, glm::vec2(float(i) / 10, 464.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::CLIMBER_UP_LEFT, 12);
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::CLIMBER_UP_LEFT, glm::vec2(float(i) / 10, 480.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::BUILDER_RIGHT, 12);
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BUILDER_RIGHT, glm::vec2(float(i) / 10, 496.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BUILDER_RIGHT, glm::vec2(float(i) / 10, 512.0f / SPRITE_HEIGHT));
		sprite->setAnimationSpeed(LemmingAnims::BUILDER_LEFT, 12);
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BUILDER_LEFT, glm::vec2(float(i) / 10, 528.0f / SPRITE_HEIGHT));
		for (int i = 0; i < 8; i++)
			sprite->addKeyframe(LemmingAnims::BUILDER_LEFT, glm::vec2(float(i) / 10, 544.0f / SPRITE_HEIGHT));

		sprite->setAnimationSpeed(LemmingAnims::FINISH_BUILDER_RIGHT, 12);
		for (int j = 496; j < 560; j += 16)
			for (int i = 8; i < 10; i++)
				sprite->addKeyframe(LemmingAnims::FINISH_BUILDER_RIGHT, glm::vec2(float(i) / 10, float(j) / SPRITE_HEIGHT));
		sprite->setAnimationSpeed(LemmingAnims::FINISH_BUILDER_LEFT, 12);		 
		for (int j = 560; j < 624; j += 16)
			for (int i = 9; i > 7; i--)
				sprite->addKeyframe(LemmingAnims::FINISH_BUILDER_LEFT, glm::vec2(float(i) / 10, float(j) / SPRITE_HEIGHT));
		
	sprite->changeAnimation(LemmingAnims::FALLING_RIGHT);
	sprite->setPosition(initialPosition);

	fallSpeed = 2.0f;
	builderNumStairs = 0;
	canBuild = false;
	builderHasToMove = false;
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
		if (pendingFloater && sprite->animationTime() > 30.0f) {
			sprite->changeAnimation(LemmingAnims::FLOATER_INI_LEFT);
			state = LemmingState::FLOATER_LEFT_STATE;
			pending_state = NULL_STATE;
		}
		else _fall(LemmingAnims::WALKING_LEFT, LemmingState::WALKING_LEFT_STATE);
		break;

	case FALLING_RIGHT_STATE:
		if (pendingFloater && sprite->animationTime() > 30.0f) {
			sprite->changeAnimation(LemmingAnims::FLOATER_INI_RIGHT);
			state = LemmingState::FLOATER_RIGHT_STATE;
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
		else if (pending_state == BASHER_LEFT_STATE) {
			sprite->changeAnimation(LemmingAnims::BASHER_LEFT);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (pending_state == DIAG_BASHER_LEFT_STATE) {
			sprite->changeAnimation(LemmingAnims::DIAG_BASHER_LEFT); 
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (pending_state == BUILDER_LEFT_STATE) {
			sprite->changeAnimation(LemmingAnims::BUILDER_LEFT);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else {
			sprite->position() += glm::vec2(-1, -4);
			if (collisionFloor() && pendingClimber) {
				sprite->changeAnimation(LemmingAnims::CLIMBER_LEFT);
				state = LemmingState::CLIMBER_LEFT_STATE;
				pending_state = NULL_STATE;
			}
			else if (collisionAny()) changeDirection();
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
		else if (pending_state == BLOCKER_RIGHT_STATE) {
			sprite->changeAnimation(LemmingAnims::BLOCKER_ANIM);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (pending_state == BASHER_RIGHT_STATE) {
			sprite->changeAnimation(LemmingAnims::BASHER_RIGHT);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (pending_state == DIAG_BASHER_RIGHT_STATE) {
			sprite->changeAnimation(LemmingAnims::DIAG_BASHER_RIGHT);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (pending_state == BUILDER_RIGHT_STATE) {
			sprite->changeAnimation(LemmingAnims::BUILDER_RIGHT);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else {
			sprite->position() += glm::vec2(1, -4);
			if (collisionFloor() && pendingClimber) {
				sprite->changeAnimation(LemmingAnims::CLIMBER_RIGHT);
				state = LemmingState::CLIMBER_RIGHT_STATE;
				pending_state = NULL_STATE;
			}
			else if (collisionAny()) changeDirection();
			else _walk(LemmingAnims::FALLING_RIGHT, LemmingState::FALLING_RIGHT_STATE);
		}
		break;

	case BASHER_RIGHT_STATE:
		// 1,2,3,4 // 7,8,9,10 // 15 // 18,19,20 // 23,24,25,26 // 31
		if (pending_state == SURRENDER_STATE) {
			sprite->changeAnimation(LemmingAnims::SURRENDER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (basherHasToMove()) {
			sprite->position() += glm::vec2(1, -1);
			if (basherCanBash(true)) {
				sprite->position() -= glm::vec2(0, -1);
				_bash();
				_walk(LemmingAnims::FALLING_RIGHT, LemmingState::FALLING_RIGHT_STATE);
				isBashing = true;
			}
			else if (collisionCollider()) changeDirection();
			else if (!isBashing && sprite->keyFrame() == 31) {
				sprite->position() -= glm::vec2(0, -1);
				sprite->changeAnimation(LemmingAnims::WALKING_RIGHT);
				state = LemmingState::WALKING_RIGHT_STATE;
			}
			else if (isBashing){
				sprite->position() -= glm::vec2(0, -1);
				sprite->changeAnimation(LemmingAnims::WALKING_RIGHT);
				state = LemmingState::WALKING_RIGHT_STATE;
				isBashing = false;
			}
			else {
				_walk(LemmingAnims::FALLING_RIGHT, LemmingState::FALLING_RIGHT_STATE);
			}
		}
		break;

	case BASHER_LEFT_STATE:
		if (pending_state == SURRENDER_STATE) {
			sprite->changeAnimation(LemmingAnims::SURRENDER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (basherHasToMove()) {
			sprite->position() += glm::vec2(-1, -1);
			if (basherCanBash(false)) {
				sprite->position() -= glm::vec2(0, -1);
				_bash();
				_walk(LemmingAnims::FALLING_LEFT, LemmingState::FALLING_LEFT_STATE);
				isBashing = true;
			}
			else if (collisionCollider()) changeDirection();
			else if (!isBashing && sprite->keyFrame() == 31) {
				sprite->position() -= glm::vec2(0, -1);
				sprite->changeAnimation(LemmingAnims::WALKING_LEFT);
				state = LemmingState::WALKING_LEFT_STATE;
			}
			else if (isBashing) {
				sprite->position() -= glm::vec2(0, -1);
				sprite->changeAnimation(LemmingAnims::WALKING_LEFT);
				state = LemmingState::WALKING_LEFT_STATE;
				isBashing = false;
			}
			else {
				_walk(LemmingAnims::FALLING_LEFT, LemmingState::FALLING_LEFT_STATE);
			}
		}
		break;

	case DIAG_BASHER_RIGHT_STATE:
		// 1,2,3,4 // 7,8,9,10 // 15 // 18,19,20 // 23,24,25,26 // 31
		if (pending_state == SURRENDER_STATE) {
			sprite->changeAnimation(LemmingAnims::SURRENDER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else _diagBash(LemmingAnims::FALLING_RIGHT, LemmingState::FALLING_RIGHT_STATE);
		
		break;

	case DIAG_BASHER_LEFT_STATE:
		if (pending_state == SURRENDER_STATE) {
			sprite->changeAnimation(LemmingAnims::SURRENDER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else _diagBash(LemmingAnims::FALLING_LEFT, LemmingState::FALLING_LEFT_STATE);

		break;

	case CLIMBER_LEFT_STATE:
		if (pending_state == SURRENDER_STATE) {
			sprite->changeAnimation(LemmingAnims::SURRENDER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (collisionFloor()) {
			if (sprite->keyFrame() > 4) sprite->position() += glm::vec2(0, -1);

			glm::vec2 pos = sprite->position() + glm::vec2(lem_offset, 0); // Add the map displacement
			pos += glm::vec2(11, 5);
			if ((mask->pixel(pos.x, pos.y) == 255) || (mask->pixel(pos.x + 1, pos.y) == 255)) {
				sprite->position() += glm::vec2(2, 1);

				sprite->changeAnimation(LemmingAnims::FALLING_RIGHT);
				state = LemmingState::FALLING_RIGHT_STATE;
				pending_state = NULL_STATE;
			}
		}
		else {
			if (sprite->animation() == LemmingAnims::CLIMBER_LEFT) {
				sprite->changeAnimation(LemmingAnims::CLIMBER_UP_LEFT);
			}
			else if (sprite->keyFrame() == 7) {
				sprite->changeAnimation(LemmingAnims::WALKING_LEFT);
				state = LemmingState::WALKING_LEFT_STATE;
				pending_state = NULL_STATE;
			}
		}
		break;

	case CLIMBER_RIGHT_STATE:
		if (pending_state == SURRENDER_STATE) {
			sprite->changeAnimation(LemmingAnims::SURRENDER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if(collisionFloor()) {
			if (sprite->keyFrame() > 4) sprite->position() += glm::vec2(0, -1);

			glm::vec2 pos = sprite->position() + glm::vec2(lem_offset, 0); // Add the map displacement
			pos += glm::vec2(5, 5);
			if ((mask->pixel(pos.x, pos.y) == 255) || (mask->pixel(pos.x - 1, pos.y) == 255)) {
				sprite->position() += glm::vec2(-2, 1);

				sprite->changeAnimation(LemmingAnims::FALLING_LEFT);
				state = LemmingState::FALLING_LEFT_STATE;
				pending_state = NULL_STATE;
			}
		}
		else {
			if (sprite->animation() == LemmingAnims::CLIMBER_RIGHT) {
				sprite->changeAnimation(LemmingAnims::CLIMBER_UP_RIGHT);
			}
			else if (sprite->keyFrame() == 7) {
				sprite->changeAnimation(LemmingAnims::WALKING_RIGHT);
				state = LemmingState::WALKING_RIGHT_STATE;
				pending_state = NULL_STATE;
			}
		}
		break;

	case BUILDER_RIGHT_STATE:
		if (pending_state == SURRENDER_STATE) {
			sprite->changeAnimation(LemmingAnims::SURRENDER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (builderNumStairs == 12) {
			sprite->changeAnimation(LemmingAnims::FINISH_BUILDER_RIGHT);
			state = FINISH_BUILDER_RIGHT_STATE;
			builderNumStairs = 0;
		}
		else if (sprite->keyFrame() <= 1 && !canBuild) {
			if (builderHasToMove) {
				sprite->position() += glm::vec2(2, -1);
				builderHasToMove = false;
			}
			sprite->position() += glm::vec2(1, -1);
			if (builderCanBuild(true))
				canBuild = true;
			sprite->position() -= glm::vec2(1, -1);
		}
		else if (canBuild) {
			if (sprite->keyFrame() == 15) {
				_build(true);
				builderNumStairs++;
				builderHasToMove = true;
				canBuild = false;
			}
		}
		else changeDirection();
		break;

	case BUILDER_LEFT_STATE:
		if (pending_state == SURRENDER_STATE) {
			sprite->changeAnimation(LemmingAnims::SURRENDER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (builderNumStairs == 12) {
			sprite->changeAnimation(LemmingAnims::FINISH_BUILDER_LEFT);
			state = FINISH_BUILDER_LEFT_STATE;
			builderNumStairs = 0;
		}
		else if (sprite->keyFrame() <= 1 && !canBuild) {
			if (builderHasToMove) {
				sprite->position() += glm::vec2(-2, -1);
				builderHasToMove = false;
			}
			sprite->position() += glm::vec2(-1, -1);
			if (builderCanBuild(false))
				canBuild = true;
			sprite->position() -= glm::vec2(-1, -1);
		}
		else if (canBuild) {
			if (sprite->keyFrame() == 15) {
				_build(false);
				builderNumStairs++;
				builderHasToMove = true;
				canBuild = false;
			}
		}
		else changeDirection();
		break;

	case FINISH_BUILDER_RIGHT_STATE:
		if (builderHasToMove) {
			sprite->position() += glm::vec2(2, -1);
			builderHasToMove = false;
		}
		if (pending_state == SURRENDER_STATE) {
			sprite->changeAnimation(LemmingAnims::SURRENDER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (pending_state == BUILDER_RIGHT_STATE) {
			sprite->changeAnimation(LemmingAnims::BUILDER_RIGHT);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (sprite->keyFrame() == 7) {
			sprite->changeAnimation(LemmingAnims::WALKING_RIGHT);
			state = LemmingState::WALKING_RIGHT_STATE;
		}
		break;

	case FINISH_BUILDER_LEFT_STATE:
		if (builderHasToMove) {
			sprite->position() += glm::vec2(-2, -1);
			builderHasToMove = false;
		}
		if (pending_state == SURRENDER_STATE) {
			sprite->changeAnimation(LemmingAnims::SURRENDER);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (pending_state == BUILDER_LEFT_STATE) {
			sprite->changeAnimation(LemmingAnims::BUILDER_LEFT);
			state = pending_state;
			pending_state = NULL_STATE;
		}
		else if (sprite->keyFrame() == 7) {
			sprite->changeAnimation(LemmingAnims::WALKING_LEFT);
			state = LemmingState::WALKING_LEFT_STATE;
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
	return (pending_state != NULL_STATE || pendingFloater);
}

bool Lemming::isBlocker()
{
	return (state == BLOCKER_RIGHT_STATE || state == BLOCKER_LEFT_STATE);
}

bool Lemming::basherHasToMove()
{
	if (sprite->keyFrame() > 0 && sprite->keyFrame() < 5 ||
		sprite->keyFrame() > 6 && sprite->keyFrame() < 11 ||
		sprite->keyFrame() == 15 || sprite->keyFrame() == 31 ||
		sprite->keyFrame() > 17 && sprite->keyFrame() < 21 ||
		sprite->keyFrame() > 22 && sprite->keyFrame() < 27)
		return true;
	return false;
}

bool Lemming::diagBasherHasToMove()
{
	if (sprite->keyFrame() == 3)
		return true;
	return false;
}

bool Lemming::builderCanBuild(bool isRight)
{
	int posX, posY;
	if (isRight) {
		posX = sprite->position().x + lem_offset + 7;
		posY = sprite->position().y + 7;

		for (int y = max(0, posY); y <= min(mask->height() - 1, posY + 8); y++) {
			for (int x = max(0, posX); x <= min(mask->width() - 1, posX + 1); x++) {
				if ((mask->pixel(x, y)) == 255)
					return false;
			}
		}
		return true;
	}
	else {
		posX = sprite->position().x + lem_offset + 8;
		posY = sprite->position().y + 7;

		for (int y = max(0, posY); y <= min(mask->height() - 1, posY + 8); y++) {
			for (int x = max(0, posX - 1); x <= min(mask->width() - 1, posX); x++) {
				if ((mask->pixel(x, y)) == 255)
					return false;
			}
		}
		return true;
	}
}

bool Lemming::basherCanBash(bool isRight)
{
	int posX, posY;
	if (isRight) {
		posX = sprite->position().x + lem_offset + 8;
		posY = sprite->position().y + 7;

		for (int y = max(0, posY); y <= min(mask->height() - 1, posY + 7); y++) {
			for (int x = max(0, posX); x <= min(mask->width() - 1, posX + 6); x++) {
				if ((mask->pixel(x, y)) == 255)
					return true;
			}
		}
		return false;
	}
	else {
		posX = sprite->position().x + lem_offset + 7;
		posY = sprite->position().y + 7;

		for (int y = max(0, posY); y <= min(mask->height() - 1, posY + 7); y++) {
			for (int x = max(0, posX - 6); x <= min(mask->width() - 1, posX); x++) {
				if ((mask->pixel(x, y)) == 255)
					return true;
			}
		}
		return false;
	}
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

	if (newSkill == LemmingSkill::FLOATER) {
		pendingFloater = true;
	}
	else if (newSkill == LemmingSkill::CLIMBER) {
		pendingClimber = true;
	}
	else {
		pending_state = getStateFromSkill(newSkill);
		pendingFloater = false;
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
		if (skill == LemmingSkill::NO_SKILL) return LemmingState::NULL_STATE;
		if (skill == LemmingSkill::BLOCKER) return LemmingState::BLOCKER_LEFT_STATE;
		if (skill == LemmingSkill::DIGGER) return LemmingState::DIGGER_LEFT_STATE;
		if (skill == LemmingSkill::FLOATER) return LemmingState::FLOATER_LEFT_STATE;
		if (skill == LemmingSkill::BASHER) return LemmingState::BASHER_LEFT_STATE;
		if (skill == LemmingSkill::DIAG_BASHER) return LemmingState::DIAG_BASHER_LEFT_STATE;
		if (skill == LemmingSkill::CLIMBER) return LemmingState::CLIMBER_LEFT_STATE;
		if (skill == LemmingSkill::BUILDER) return LemmingState::BUILDER_LEFT_STATE;
		else cout << "You have to implement this skill on Lemming::getStateFromSkill(LemmingSkill skill); !" << endl;
	}
	else {
		if (skill == LemmingSkill::NO_SKILL) return LemmingState::NULL_STATE;
		if (skill == LemmingSkill::BLOCKER) return LemmingState::BLOCKER_RIGHT_STATE;
		if (skill == LemmingSkill::DIGGER) return LemmingState::DIGGER_RIGHT_STATE;
		if (skill == LemmingSkill::FLOATER) return LemmingState::FLOATER_RIGHT_STATE;
		if (skill == LemmingSkill::BASHER) return LemmingState::BASHER_RIGHT_STATE;
		if (skill == LemmingSkill::DIAG_BASHER) return LemmingState::DIAG_BASHER_RIGHT_STATE;
		if (skill == LemmingSkill::CLIMBER) return LemmingState::CLIMBER_RIGHT_STATE;
		if (skill == LemmingSkill::BUILDER) return LemmingState::BUILDER_RIGHT_STATE;
		else cout << "You have to implement this skill on Lemming::getStateFromSkill(LemmingSkill skill); !" << endl;
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
	if (newSkill == BUILDER && (state == BUILDER_RIGHT_STATE || state == BUILDER_LEFT_STATE)) return true;
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

void Lemming::_climb(LemmingAnims nextAnim, LemmingState nextState) {

}

void Lemming::_walk(LemmingAnims fallAnimation, LemmingState fallState)
{
	int fall = collisionAny(9);
	if (fall > 0)
		sprite->position() += glm::vec2(0, fall);
	if (fall > 8) {
		sprite->changeAnimation(fallAnimation);
		state = fallState;
		isBashing = false;
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

void Lemming::_bash()
{

	int posX, posY;

	posX = sprite->position().x + lem_offset;
	if (sprite->animation() == LemmingAnims::BASHER_RIGHT) posX += 8;
	else posX += 6;
	posY = sprite->position().y + 11;
	int r = 4;
	int dx;
	int dy;
	for (int y = max(0, posY - r); y <= min(mask->height() - 1, posY + r); y++) {
		for (int x = max(0, posX - r); x <= min(mask->width() - 1, posX + r); x++) {
			dx = posX - x + 1; // horizontal offset
			dy = posY - y; // vertical offset
			if ((dx*dx + dy * dy) <= (r*r))
			{
				mask->setPixel(x, y, 0);
			}
		}
	}
}

void Lemming::_diagBash(LemmingAnims fallAnim, LemmingState fallState)
{
	int fall = collisionAny(3);
	if (fall > 0) {
		sprite->position() += glm::vec2(0, 1);
	}
	if (fall > 1) {
		sprite->changeAnimation(fallAnim);
		state = fallState;
		pending_state = LemmingState::NULL_STATE;
	}
	else if (sprite->keyFrame() == 2) {
		int posX, posY;
		int r = 6;
		int dx;
		int dy;
		if (state == DIAG_BASHER_RIGHT_STATE) {
			posX = int(sprite->position().x) + lem_offset + 12;
			posY = int(sprite->position().y) + 12;			
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
			sprite->position() += glm::vec2(4, 2);
		}
		else {
			cout << r << endl;
			posX = int(sprite->position().x) + lem_offset + 3;
			posY = int(sprite->position().y) + 12;
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
			sprite->position() += glm::vec2(-4, 2);
		}
	}
}

void Lemming::_explote()
{
	int posX, posY;

	posX = sprite->position().x + lem_offset + 7;
	posY = sprite->position().y + 13;
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

void Lemming::_build(bool isRight)
{
	int posX, posY;
	if (isRight) {
		posX = sprite->position().x + lem_offset + 9;
		posY = sprite->position().y + 15;

		for (int x = max(0, posX); x <= min(mask->width() - 1, posX + 3); x++) {
			mask->setPixel(x, posY, 255);
		}
	}
	else {
		posX = sprite->position().x + lem_offset + 6;
		posY = sprite->position().y + 15;

		for (int x = max(0, posX - 3); x <= min(mask->width(), posX); x++) {
			mask->setPixel(x, posY, 255);
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
		sprite->position() -= glm::vec2(-1, -3);
		sprite->changeAnimation(LemmingAnims::WALKING_RIGHT);
		state = WALKING_RIGHT_STATE;
	}
	else if (state == WALKING_RIGHT_STATE) {
		sprite->position() -= glm::vec2(1, -3);
		sprite->changeAnimation(LemmingAnims::WALKING_LEFT);
		state = WALKING_LEFT_STATE;
	}
	else if (state == BASHER_LEFT_STATE) {
		sprite->position() -= glm::vec2(-1, -1);
		sprite->changeAnimation(LemmingAnims::BASHER_RIGHT);
		state = BASHER_RIGHT_STATE;
	} 
	else if (state == BASHER_RIGHT_STATE) {
		sprite->position() -= glm::vec2(1, -1);
		sprite->changeAnimation(LemmingAnims::BASHER_LEFT);
		state = BASHER_LEFT_STATE;
	}
	else if (state == BUILDER_LEFT_STATE) {
		sprite->changeAnimation(LemmingAnims::WALKING_RIGHT);
		state = WALKING_RIGHT_STATE;
	}
	else if (state == BUILDER_RIGHT_STATE) {
		sprite->changeAnimation(LemmingAnims::WALKING_LEFT);
		state = WALKING_LEFT_STATE;
	}
}

void Lemming::incrementFallSpeed() {
	if (fallSpeed < MAXIMUM_FALL_SPEED)  fallSpeed += GRAVITY;
	else fallSpeed = MAXIMUM_FALL_SPEED;
}



