#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <map>

class AudioManager {
private:
	explicit AudioManager() {}
	AudioManager(const AudioManager &ref) {}
	AudioManager & operator =(const AudioManager &ref) {}
	~AudioManager();
	void clear();
	std::map<std::string, Mix_Music*> musicMap;
	std::map<std::string, Mix_Chunk*> soundMap;

public:
	inline static AudioManager &Instance() {
		static AudioManager singleton;
		return singleton;
	}

	void initAudio();

	bool loadMusic(std::string file, std::string id);
	bool loadSound(std::string file, std::string id);
	void playMusic(std::string id);
	void playSound(std::string id);
};