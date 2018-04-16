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

	enum buttonAnim
	{
		NORMAL_DIGGER, SELECT_DIGGER, NORMAL_FLOATER, SELECT_FLOATER, INCREASE_DECREASE_RATING, 
		NORMAL_CLIMBER, SELECT_CLIMBER, NORMAL_EXPLOSIVE, SELECT_EXPLOSIVE, NORMAL_BLOCKER, SELECT_BLOCKER,
		NORMAL_BUILDER, SELECT_BUILDER, NORMAL_BASHER, SELECT_BASHER, NORMAL_DIAG_BASHER, SELECT_DIAG_BASHER,
		NORMAL_PAUSE, SELECT_PAUSE, NORMAL_SURREND, SELECT_SURREND, NORMAL_FAST, SELECT_FAST
	};

	bool isSelect();
	int getAnim();
	void setAnim(buttonAnim anim);
	void setSelect(bool selection);

private:
	const float SPRITE_HEIGHT = 390.0f;

	Texture spritesheet;
	Sprite *sprite;
	float sprite_h;
};


#endif // _BUTTON_INCLUDE
