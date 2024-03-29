#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Quad.h"
#include "Texture.h"
#include "Lemming.h"
#include "cursor.h"
#include "Button.h"
#include "Text.h"




class Menu
{
public:
	Menu();
	~Menu();
	void init(Cursor *cursor);
	int update(int deltaTime);
	void render();

	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

	void resetTransition();

private:
	void initShaders();
	void initButtons();
	void checkButtons(int x, int y);

private:
	Texture colorTexture, titleTexture, lemmingTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	ShaderProgramQuad program;
	float currentTime;
	glm::mat4 projection;

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

	int transition = 0;
};

#endif // _MENU_INCLUDE