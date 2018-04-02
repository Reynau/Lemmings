#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "cursor.h"


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

public:
	VariableTexture maskTexture;

private:
	void initShaders();
	void initLevels();
	//void eraseMask(int mouseX, int mouseY);
	//void applyMask(int mouseX, int mouseY);
	void changeLevel(int level);
	void finishLevel();

	void checkSelecting();
	void applySkill(Lemming::LemmingState skill);

	void checkIfLemmingSafe(int lemmingId);
	bool lemmingHasToSpawn();
	bool lemmingColideWith(Lemming * lemming, glm::vec2 startPoint, glm::vec2 endPoint);

	void initLemmings();
	void clearLemmings();
	void resetLemmings();
	void removeLemming(int lemmingId);

	int considerSceneSpeed(int deltaTime);

private:
	Texture colorTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	float currentTime;
	glm::mat4 projection;

	// Level related
	struct Level {
		string name;
		int lemmingsToSpawn;
		int lemmingsToSecure;
		int availableTime;
		glm::vec2 spawnPosition;
		glm::vec2 savePosition;
		string colorTextureFile;
		string maskTextureFile;
		float offset;
	};
	vector<Level> levels;
	int currentLevel;

	// Lemming related
	Texture lemmingTexture;
	vector<Lemming *> lemmings;	
	int spawnedLemmings;
	int aliveLemmings;
	int safeLemmings;

	// Cursor related
	cursor * cur;
	int index_selected_lem;

	// Scene Speed
	enum SceneSpeed {
		PAUSE,
		NORMAL,
		FAST,
	};
	SceneSpeed sceneSpeed;
};


#endif // _SCENE_INCLUDE

