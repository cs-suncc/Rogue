#include "AudioManager.h"
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

AudioManager::~AudioManager() {
	clear();
}

void AudioManager::clear() {
	for (auto each : musicMap)
		Mix_FreeMusic(each.second);
	for (auto each : soundMap)
		Mix_FreeChunk(each.second);
}

void AudioManager::initAudio() {
	if (!SDL_WasInit(SDL_INIT_AUDIO)) {
		SDL_Init(SDL_INIT_AUDIO);
	}
	Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
	Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024);
	Mix_AllocateChannels(32);
	Mix_Volume(-1, 32);
}

bool AudioManager::loadMusic(std::string file, std::string id) {
	auto t = Mix_LoadMUS(file.c_str());
	SDL_assert(t != nullptr);
	if (t == nullptr) {
		return false;
	}
	if (musicMap.find(id) != musicMap.end()) {
#ifdef _DEBUG
		std::cerr << "Music " << id << " has been loaded before" << std::endl;
#endif // _DEBUG
	} else {
		musicMap[id] = t;
	}
	return true;
}

bool AudioManager::loadSound(std::string file, std::string id) {
	auto t = Mix_LoadWAV(file.c_str());
	SDL_assert(t != nullptr);
	if (t == nullptr) {
		return false;
	}
	if (soundMap.find(id) != soundMap.end()) {
#ifdef _DEBUG
		std::cerr << "Sound " << id << " has been loaded before" << std::endl;
#endif // _DEBUG
	} else {
		soundMap[id] = t;
	}
	return true;
}

void AudioManager::playMusic(std::string id) {
	Mix_PlayMusic(musicMap[id], -1);
}

void AudioManager::playSound(std::string id) {
	Mix_PlayChannelTimed(-1, soundMap[id], 0, -1);
}
