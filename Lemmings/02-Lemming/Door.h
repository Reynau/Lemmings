#ifndef _DOOR_INCLUDE
#define _DOOR_INCLUDE

#include "Sprite.h"


// Door is basically a Sprite that represents the start and finish doors.


class Door
{
public:
	void initDoor(ShaderProgram &shaderProgram);
	void setPos(glm::vec2 pos);
	void update(int deltaTime);
	void render();

	enum doorState
	{
		FIRST_DOOR, SECOND_DOOR
	};

	void setState(doorState door_state);

private:
	const float SPRITE_HEIGHT = 96.0f;

	Texture spritesheet;
	Sprite *sprite;
	float sprite_h;

	doorState state;
	
};

#endif // _DOOR_INCLUDE