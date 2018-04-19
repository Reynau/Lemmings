#include "Audio.h"

Audio::Audio()
{
	result = FMOD::System_Create(&system);
	ERRCHECK(result);

	result = system->getVersion(&version);
	ERRCHECK(result);

	if (version < FMOD_VERSION)
	{
		printf("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
		exit(-1);
	}

	result = system->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	ERRCHECK(result);
}


Audio::~Audio()
{
}

void Audio::loadAudio(const char* path) {
	FMOD::Sound *sound;
	result = system->createSound(path, FMOD_DEFAULT, 0, &sound);
	ERRCHECK(result);

	sounds[path] = sound;
}
void Audio::playAudio(const char* path) {
	FMOD::Sound *sound = sounds[path];
	system->playSound(sound, NULL, false, &channel);
}

void Audio::removeAudio(const char* path) {
	sounds[path]->release();
}

void Audio::update() {
	result = system->update();
	ERRCHECK(result);
}

void Audio::ERRCHECK(FMOD_RESULT result) {
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

