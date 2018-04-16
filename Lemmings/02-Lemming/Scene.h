#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "cursor.h"
#include "Door.h"
#include "Quad.h"
#include "Countdown.h"
#include "Button.h"


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
	void moveMap(bool right);

public:
	VariableTexture maskTexture;

private:
	void initShaders();
	void initLevels();

	void updateColliders();

	//void eraseMask(int mouseX, int mouseY);
	//void applyMask(int mouseX, int mouseY);

	void changeLevel(int level);
	void finishLevel();
	void resetOffsets();

	bool checkSelecting();
	void checkButtons();
	void applyButtonPressed(int i);
	void undoButtonPressed(int i);
	void applySkill(Lemming::LemmingSkill skill);
	void pressButton();

	void checkIfLemmingSafe(int lemmingId);
	bool lemmingHasToSpawn(int deltaTime);
	bool lemmingColideWith(Lemming * lemming, glm::vec2 startPoint, glm::vec2 endPoint);

	void initLemmings();
	void clearLemmings();
	void clearCounts();
	void resetLemmings();
	void resetButtons();
	void removeLemming(int lemmingId);
	void removeCount(int countId);

	int considerSceneSpeed(int deltaTime);

	void setSurrender();
	void countdownLemming(int i);
	void exploteLemming(int i);

	void initButtons();

private:
	Texture colorTexture;
	VariableTexture *maskTexture2;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	ShaderProgramQuad program;
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
		int spawnTime;
		Door::doorState door;
	};
	vector<Level> levels;
	int currentLevel;

	// Lemming related
	Texture lemmingTexture;
	vector<Lemming *> lemmings;
	vector<glm::vec2> colliders;
	int spawnedLemmings;
	int aliveLemmings;
	int safeLemmings;

	// Cursor related
	Cursor * cursor;
	int index_selected_lem;
	int index_selected_but;

	// Door related
	Door * spawnDoor;
	Door * door;
	bool lemmingsNotUpdated;

	// Scene Speed
	enum SceneSpeed {
		PAUSE,
		NORMAL,
		FAST,
	};
	SceneSpeed sceneSpeed;

	// Border related
	Quad *quad;
	Quad *quadui;

	// Surrender countdown related
	vector<Countdown *> countds;
	bool surrStarted;
	float surrTime;

	// UI related
	Texture buttonTexture;
	vector<Button *> buttons;
	int selected_but;
	Lemming::LemmingSkill selected_lem_state;
};


#endif // _SCENE_INCLUDE

