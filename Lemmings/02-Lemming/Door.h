#ifndef _DOOR_INCLUDE
#define _DOOR_INCLUDE

#include "Sprite.h"


// Door is basically a Sprite that represents the start and finish doors.


class Door
{
public:
	void initDoor(ShaderProgram &shaderProgram);
	//glm::vec2 getPos();
	void setPos(glm::vec2 pos);
	void render();

	enum doorState
	{
		FIRST_DOOR
	};

private:
	Texture spritesheet;
	Sprite *sprite;
	float sprite_h;
	
};

#endif // _DOOR_INCLUDE