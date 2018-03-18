#include <windows.h>
#include <mmsystem.h>
#include <mciapi.h>
#include<string>
#include"SoundManager.h"
#pragma once

enum FX {
	STEP, TAKE_SWORD, ATTACK_SWORD, DRINK_POTION, DEATH, FIGHT_DEATH,
	WON, SWORD_FIGHT1, SWORD_FIGHT2, SWORD_FIGHT3, DOOR_OPENING,
	CHAINDOOR_OPENING, JUMP, JUMP_UP, JUMP_DOWN
};

class SoundFX
{
public:
	
	
	SoundFX();
	~SoundFX();

	void PlayThemeSong();
	void PlayWinSong();
	void PlayFX(FX FXid);
};
