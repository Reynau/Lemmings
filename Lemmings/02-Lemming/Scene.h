#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	
	void mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	void initShaders();
	void initLevels();
	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);
	void changeLevel(int level);

private:
	Texture colorTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	float currentTime;
	glm::mat4 projection;
	Lemming lemming;

	struct Level {
		string name;
		int lemmingsToSpawn;
		int lemmingsToSave;
		int availableTime;
		glm::vec2 spawnPosition;
		glm::vec2 savePosition;
		string colorTextureFile;
		string maskTextureFile;
	};
	vector<Level> levels;
	int actualLevel;

	vector<Lemming> lemmings;
	int spawnedLemmings;
};


#endif // _SCENE_INCLUDE

