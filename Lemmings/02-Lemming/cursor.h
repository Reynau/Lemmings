#ifndef _CURSOR_INCLUDE
#define _CURSOR_INCLUDE

#include "Sprite.h"
#include "Lemming.h"


// Cursor is basically a Sprite that represents the cursor. As such it has
// all properties it needs to track its movement.


class Cursor
{
public:
	void initCursor(ShaderProgram &shaderProgram, Texture * lemmingTexture);
	glm::vec2 getPos();
	void setPos(int mouseX, int mouseY);
	void update(int deltaTime);
	void render();

	void setSelect(bool selecting);

	enum cursorState
	{
		NORMAL, SELECT
	};

private:
	const float SPRITE_HEIGHT = 624.0f;

	Texture spritesheet;
	Sprite *sprite;
	float sprite_h;

	cursorState state;
};

#endif // _CURSOR_INCLUDE