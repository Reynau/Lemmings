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

class Credits
{
public:
	Credits();
	~Credits();

	void init(Cursor *cursor);
	int update(int deltaTime);
	void render();

	void resetTransition();

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

	int transition = 0;
};

