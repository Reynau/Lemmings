#include "SoundFX.h"
#include<iostream>
#pragma comment(lib, "winmm.lib")
using namespace std;

SoundFX::SoundFX()
{
}


SoundFX::~SoundFX()
{
}



void SoundFX::PlayThemeSong() {
	Sound::initialise();
	mciSendString((LPCWSTR)L"open sounds\\theme.mp3 type mpegvideo alias themesong", NULL, 0, NULL);
	mciSendString((LPCWSTR)L"play themesong repeat", NULL, 0, NULL);
}

void SoundFX::PlayWinSong() {
	mciSendString((LPCWSTR)L"stop themesong", NULL, 0, NULL);
	mciSendString((LPCWSTR)L"open sounds\\won_light.mp3 type mpegvideo alias wonsong", NULL, 0, NULL);
	mciSendString((LPCWSTR)L"play wonsong repeat", NULL, 0, NULL);
}


void SoundFX::PlayFX(FX FXid) {
	switch (FXid) {
	case DRINK_POTION:
		Sound::load("sounds\\potion.wav");
		break;
	case DEATH:
		Sound::load("sounds\\regular_death.wav");
		break;
	case TAKE_SWORD:
		Sound::load("sounds\\sword_found.wav");
		break;
	case STEP:
		Sound::load("sounds\\step.wav");
		break;
	case FIGHT_DEATH:
		Sound::load("sounds\\fight_death.wav");
		break;
	case SWORD_FIGHT1:
		Sound::load("sounds\\sword_fight_1.wav");
		break;
	case SWORD_FIGHT2:
		Sound::load("sounds\\sword_fight_2.wav");
		break;
	case SWORD_FIGHT3:
		Sound::load("sounds\\sword_fight_3.wav");
		break;
	case DOOR_OPENING:
		Sound::load("sounds\\door_opening.wav");
		break;
	case CHAINDOOR_OPENING:
		Sound::load("sounds\\chaindoor_opening.wav");
		break;
	case JUMP :
		Sound::load("sounds\\jump.wav");
		break;
	case JUMP_UP:
		Sound::load("sounds\\jump_up.wav");
		break;
	case JUMP_DOWN:
		Sound::load("sounds\\jump_down.wav");
		break;
	}
	Sound::play();
}

