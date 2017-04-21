#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDLGameObject.h"
#include <vector>
#include <exception>

class Game {
private:
	Game() :window(nullptr), renderer(nullptr), gameRunning(false), hottime(0) {}
	~Game() {
		quit();
	}
	Game(const Game &ref) {}	//Singleton
	const Game & operator= (const Game &ref) {}

	void loadAssets();

	SDL_Window *window;
	SDL_Renderer *renderer;
	bool gameRunning;
	int hottime;
public:
	inline static Game &Instance() {
		static Game singleton;	//Singleton
		return singleton;
	}
	bool init(const char *title, int x, int y, int width, int height, int flags = SDL_WINDOW_SHOWN);
	void quit();
	bool running() const { return gameRunning; }
	SDL_Renderer* getRenderer() const { return renderer; }
	void handleEvents();
	void render();
	void update();
	void clean();
	std::vector<SDLGameObject *> gameObjects;

	int getHotTime() {
		return hottime;
	}
	void reset() {
		gameObjects.clear();
		hottime = 0;
	}
	std::string current_right_aside;
};
