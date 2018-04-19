#pragma once

#include <stdio.h>
#include <iostream>
#include <map>

#include <fmod.hpp>
#include <fmod_errors.h>

using namespace std;

class Audio
{
public:
	Audio();
	~Audio();

	void loadAudio(const char* path);
	void playAudio(const char* path);
	void removeAudio(const char* path);
	void update();

private:
	FMOD::System     *system;
	FMOD::Channel    *channel = 0;
	FMOD_RESULT       result;
	unsigned int      version;

	map<string, FMOD::Sound*> sounds;

	void ERRCHECK(FMOD_RESULT result);
};

