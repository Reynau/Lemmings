#ifndef _INSTRUCTIONS_INCLUDE
#define _INSTRUCTIONS_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Quad.h"
#include "Texture.h"
#include "Sprite.h"
#include "cursor.h"
#include "Text.h"

class Instructions
{
public:
	Instructions();
	~Instructions();
	void init(Cursor *cursor);
	int update(int deltaTime);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

	void resetTransition();

private:
	void initShaders();

private:
	Texture titleTexture, colorTexture, lemmingTexture, textureW, textureS, textureA, textureD;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	ShaderProgramQuad program;
	float currentTime;
	glm::mat4 projection;

	Sprite *sprite;

	Cursor * cursor;
	float wid, hei, xOffset, yOffset, spaceBut;
	int sizeFont;
	Text text;

	bool clicked;

	int transition = 0;

};

#endif // _INSTRUCTIONS_INCLUDE
