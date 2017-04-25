#include <iostream>
#include "playing.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "Game.h"
#include "SDLGameObject.h"
#include "MapLoader.h"

const std::string PlayingState::playingID = "MAINMENU";

void PlayingState::update() {
	InputHandler::Instance().update();
	if (InputHandler::Instance().buttonState(0, XBoxInputNodes::BUTTON_BACK)) {
		Game::Instance().getGameStateMachine()->popState();
	}
}

void PlayingState::render() {
	SDL_RenderClear(Game::Instance().getRenderer());
	auto w = mp->width;
	auto h = mp->height;
	for (auto i = 0; i < h; ++i)
		for (auto j = 0; j < w; ++j) {
			mp->_tile->draw(mp->layer[i][j], 32 * j, 32 * i);
		}
	for (auto obj : gameObjects) {
		obj->draw();
	}
}

bool PlayingState::onEnter() {
	mp = new Map("asset/test.tmx");
	SDL_SetRenderDrawColor(Game::Instance().getRenderer(), 227, 227, 227, 255);
	return true;
}

bool PlayingState::onExit() {
	for (auto obj : gameObjects) {
		obj->clean();
	}
	gameObjects.clear();
	delete mp;
#ifdef _DEBUG
	std::cerr << "Playing state - EXIT" << std::endl;
#endif
	return true;
}