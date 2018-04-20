#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Quad.h"
#include "Texture.h"
#include "TexturedQuad.h"
#include "Lemming.h"
#include "cursor.h"
#include "Door.h"
#include "Quad.h"
#include "Countdown.h"
#include "Button.h"
#include "Text.h"




class Menu
{
public:
	Menu();
	~Menu();
	void init();
	void update(int deltaTime);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	void initShaders();
	void initButtons();
	void checkButtons(int x, int y);

private:
	Texture colorTexture, titleTexture, lemmingTexture;
	VariableTexture maskTexture;
	TexturedQuad *background;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	ShaderProgramQuad program;
	float currentTime;
	glm::mat4 projection;

	Texture backgroundTexture;

	float wid, hei, xOffset, yOffset, spaceBut;
	int sizeFont;
	Text text;

	Sprite *sprite;
	Quad *quad;

	Cursor * cursor;
	int index_selected_but;
	vector<glm::vec2> buttonsPos;
	bool buttonsStarted;
	bool clicked;
};

#endif // _MENU_INCLUDE