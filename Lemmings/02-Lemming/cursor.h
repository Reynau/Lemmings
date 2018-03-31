#ifndef _CURSOR_INCLUDE
#define _CURSOR_INCLUDE

#include "Sprite.h"


// Cursor is basically a Sprite that represents the cursor. As such it has
// all properties it needs to track its movement.


class cursor
{
public:
	void initCursor(ShaderProgram &shaderProgram);
	void setPos(int mouseX, int mouseY);
	void update(int deltaTime);
	void render();

private:
	Texture spritesheet;
	Sprite *sprite;
	float sprite_h;
};

#endif // _CURSOR_INCLUDE