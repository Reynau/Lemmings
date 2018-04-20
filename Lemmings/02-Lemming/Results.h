#ifndef _RESULTS_INCLUDE
#define _RESULTS_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Quad.h"
#include "Texture.h"
#include "Sprite.h"
#include "Text.h"

class Results
{
public:
	Results();
	~Results();
	void init();
	void update(int deltaTime);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);
private:
	void initShaders();

private:
	Texture titleTexture, colorTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	ShaderProgramQuad program;
	float currentTime;
	glm::mat4 projection;

	Sprite *sprite;

	float wid, hei, xOffset, yOffset, spaceBut;
	int sizeFont;
	Text text;

	bool clicked;

};

#endif // _MENU_INCLUDE