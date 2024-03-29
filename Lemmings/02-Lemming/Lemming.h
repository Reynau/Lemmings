#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"
#include "Audio.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{
public:
	enum LemmingSkill 
	{
		CLIMBER, FLOATER, SURREND, BLOCKER, BUILDER, BASHER, DIAG_BASHER, DIGGER, NO_SKILL
	};

	enum LemmingState
	{
		NULL_STATE, WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, FALL_DIE_STATE,
		BLOCKER_RIGHT_STATE, BLOCKER_LEFT_STATE, DIGGER_RIGHT_STATE, DIGGER_LEFT_STATE, FLOATER_RIGHT_STATE, FLOATER_LEFT_STATE,
		CLIMBER_RIGHT_STATE, CLIMBER_LEFT_STATE, BASHER_RIGHT_STATE, BASHER_LEFT_STATE, DIAG_BASHER_RIGHT_STATE, DIAG_BASHER_LEFT_STATE,
		BUILDER_RIGHT_STATE, BUILDER_LEFT_STATE, FINISH_BUILDER_RIGHT_STATE, FINISH_BUILDER_LEFT_STATE,
		DEAD_STATE, ARRIVE_STATE, SAFE_STATE, SURRENDER_STATE, EXPLOSION_STATE
	};

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, Texture * lemmingTexture, Audio *audioDriver);
	void update(int deltaTime, int offset, vector<glm::vec2> colliders);
	void render();

	bool isDead();
	bool isArriving();
	bool isSafe();
	bool isBusy();
	bool isCLimber();
	bool isFloater();
	bool isBlocker();
	bool hasExploted();
	bool basherHasToMove();
	bool diagBasherHasToMove();
	bool builderCanBuild(bool isRight);
	bool basherCanBash(bool isRight);

	void remove();

	void setMapMask(VariableTexture *mapMask);
	void setPos(glm::vec2 pos);

	glm::vec2 getPosition();
	bool setSkill(LemmingSkill newSkill);
	void lemmingArrived();
	
private:
	const float SPRITE_HEIGHT = 624.0f;

	const float GRAVITY = 0.045f;
	const float MAXIMUM_FALL_SPEED = 3.0f;
	const float MINIMUM_FALL_SPEED = 2.0f;

	Sprite *sprite;
	VariableTexture *mask;

	LemmingState state;
	LemmingState pending_state;
	bool pendingFloater;
	bool pendingClimber;
	bool canBuild;
	bool builderHasToMove;
	int builderNumStairs;
	bool isBashing;
	float fallSpeed;
	int lem_offset;

	vector<glm::vec2> colliders;

	Audio *audioDriver;

	enum LemmingAnims
	{
		WALKING_LEFT, WALKING_RIGHT, FALLING_LEFT, FALLING_RIGHT, 
		DIGGING, FLOATER_INI_RIGHT, FLOATER_INI_LEFT, FLOATER_RIGHT, FLOATER_LEFT, 
		CLIMBER_RIGHT, CLIMBER_LEFT, CLIMBER_UP_RIGHT, CLIMBER_UP_LEFT, FALL_DIE, BLOCKER_ANIM,
		SAFE, SURRENDER, EXPLOSION, DIAG_BASHER_RIGHT, DIAG_BASHER_LEFT, BASHER_RIGHT, BASHER_LEFT,
		BUILDER_RIGHT, BUILDER_LEFT, FINISH_BUILDER_RIGHT, FINISH_BUILDER_LEFT
	};

private:
	void _block(LemmingAnims fallAnim, LemmingState fallState);
	void _dig(LemmingAnims fallAnim, LemmingState fallState);
	void _climb(LemmingAnims nextAnim, LemmingState nextState);
	void _walk(LemmingAnims fallAnimation, LemmingState fallState);
	void _float(LemmingAnims walkAnimation, LemmingState walkState);
	void _fall(LemmingAnims walkAnimation, LemmingState walkState);
	void _bash();
	void _diagBash(LemmingAnims fallAnim, LemmingState fallState);
	void _build(bool isRight);
	void _explote();

	int collisionAny(int maxFall);
	int collisionFloor(int maxFall);
	int collisionCollider(int maxFall);
	bool collisionAny();
	bool collisionFloor();
	bool collisionCollider();
	bool collisionClimber();
	bool collideWithBlocker(glm::vec2 startBox1, glm::vec2 endBox1, glm::vec2 startBox2, glm::vec2 endBox2);

	bool isSameSkill(LemmingSkill newState);
	bool isGoingLeft();
	bool isGoingRight();

	LemmingState getStateFromSkill(LemmingSkill skill);

	void changeDirection();
	void incrementFallSpeed();

};


#endif // _LEMMING_INCLUDE
