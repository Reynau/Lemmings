#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, Texture * lemmingTexture);
	void update(int deltaTime);
	void render();

	bool isDead();
	void remove();
	bool occupied();

	void setMapMask(VariableTexture *mapMask);

	glm::vec2 getPosition();

	enum LemmingState
	{
		NULL_STATE, WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, FALL_DIE_STATE,
		BLOCKER_STATE, DIGGER_RIGHT_STATE, DIGGER_LEFT_STATE, FLOATER_RIGHT_STATE, FLOATER_LEFT_STATE,
		DEAD_STATE
	};

	bool setState(LemmingState state);
	
private:
	void dig();

	int collisionFloor(int maxFall);
	bool collision();

	void incrementFallSpeed();
	
private:
	const float SPRITE_HEIGHT = 160.0f;

	const float GRAVITY = 0.05f;
	const float MAXIMUM_FALL_SPEED = 3.0f;
	const float MINIMUM_FALL_SPEED = 2.0f;

	LemmingState state;
	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	LemmingState pending_state;
	bool pending_floater;
	float fallSpeed;

};


#endif // _LEMMING_INCLUDE
