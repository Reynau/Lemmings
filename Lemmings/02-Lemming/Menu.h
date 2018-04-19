#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
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


private:
	void initShaders();

private:
	Texture colorTexture;
	TexturedQuad *background;
	ShaderProgramQuad program;
	float currentTime;
	glm::mat4 projection;

	Texture backgroundTexture;

	float wid, hei;
	int sizeFont;
	Text text;


	Quad *quad;
};

#endif // _MENU_INCLUDE