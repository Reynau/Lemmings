#ifndef _COUNTDOWN_INCLUDE
#define _COUNTDOWN_INCLUDE

#include "Sprite.h"
#include "Lemming.h"

class Countdown
{
public:
	void initCountdown(glm::vec2 lemPosition, ShaderProgram & shaderProgram, Texture * lemmingTexture);
	bool update(glm::vec2 lemPosition, int deltaTime);
	void render();
	void remove();
	void setOn();

	enum countdownState
	{
		ON, OFF, NULL_STATE
	};

private:
	const float SPRITE_HEIGHT = 624.0f;

	Texture spritesheet;
	Sprite *sprite;
	float sprite_h;

	countdownState state;
};


#endif // _COUNTDOWN_INCLUDE