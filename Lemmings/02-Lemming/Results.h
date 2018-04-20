#ifndef _RESULTS_INCLUDE
#define _RESULTS_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Quad.h"
#include "Texture.h"
#include "Sprite.h"
#include "Text.h"
#include "cursor.h"

class Results
{
public:
	Results();
	~Results();
	void init(Cursor *cursor);
	int update(int deltaTime);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

	void setData(int lemmingsToSave, int savedLemmings, bool winner);

	void resetTransition();
private:
	void initShaders();

private:
	Texture titleTexture, colorTexture, lemmingTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	ShaderProgramQuad program;
	float currentTime;
	glm::mat4 projection;

	Sprite *sprite;

	Cursor *cursor;

	float wid, hei, xOffset, yOffset, spaceBut;
	int sizeFont;
	Text text;

	bool clicked;

	int updateState;

	int lemmingsToSave;
	int savedLemmings;
	bool winner;

};

#endif // _MENU_INCLUDE