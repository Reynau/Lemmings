#ifndef _BUTTON_INCLUDE
#define _BUTTON_INCLUDE


#include "Sprite.h"


// Given a point (x, y) and a size (width, height) in pixels Button creates 
// a Button with 5 quads.


class Button
{
public:
	void initButton(ShaderProgram &shaderProgram, Texture * buttonTexture);
	glm::vec2 getPos();
	void setPos(float x, float y);
	void update(int deltaTime);
	void render();

	void setSelect(bool selecting);

	enum buttonState
	{
		NORMAL, SELECT
	};

private:
	const float SPRITE_HEIGHT = 624.0f;

	Texture spritesheet;
	Sprite *sprite;
	float sprite_h;

	buttonState state;
};


#endif // _BUTTON_INCLUDE
