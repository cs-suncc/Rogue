#include "Game.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "AudioManager.h"
#include <iostream>
#include <exception>
#include <cmath>

bool Game::init(const char * title, int x, int y, int width, int height, int flags) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "[Info] SDL Init Success" << std::endl;
		//��ʼ����Ϸ���ں�renderer
		window = SDL_CreateWindow(title, x, y, width, height, flags);
		if (window == nullptr) {
			std::cerr << "[Fatal] SDL failed to create window. Abort." << std::endl;
			return false;
		}
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == nullptr) {
			std::cerr << "[Fatal] SDL failed to create renderer. Abort." << std::endl;
			return false;
		}
	}
	IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
	AudioManager::Instance().initAudio();
	InputHandler::Instance().initJoysticks();
	loadAssets();
	gameRunning = true;
	return true;
}

void Game::quit() {
	InputHandler::Instance().clean();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	gameRunning = false;
}

void Game::handleEvents() {
	InputHandler::Instance().update();
}

void Game::render() {
	SDL_RenderPresent(renderer);
}

int Gdx = 0, Gdy = -1440;

void Game::update() {
	SDL_RenderClear(renderer);

}

void Game::clean() {
}

void Game::loadAssets() {

}