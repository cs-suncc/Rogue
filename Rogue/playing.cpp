#include <iostream>
#include "playing.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "Game.h"
#include "SDLGameObject.h"
#include "Player.h"
#include "GameObjectFactory.h"
#include "MapManager.h"

const std::string PlayingState::playingID = "MAINMENU";

void PlayingState::update() {
	InputHandler::Instance().update();
	if (InputHandler::Instance().buttonState(0, XBoxInputNodes::BUTTON_BACK)) {
		Game::Instance().getGameStateMachine()->popState();
		return;
	}
	for (auto obj : gameObjects) {
		obj->update();
	}
}

void PlayingState::render() {
	SDL_RenderClear(Game::Instance().getRenderer());
	MapManager::Instance().getMap(currentMap)->draw();
	for (auto obj : gameObjects) {
		obj->draw();
	}
}

bool PlayingState::onEnter() {
	MapManager::Instance().loadMap("TESTMAP", "asset/test.tmx");
	currentMap = "TESTMAP";
	TextureManager::Instance().load("asset/image/rogue2.png", "PLAYER", Game::Instance().getRenderer());
	SDL_SetRenderDrawColor(Game::Instance().getRenderer(), 227, 227, 227, 255);
	auto *player = (Player *)Game::Instance().factories().create("Player");
	player->load(LoaderParams(0 * 32, 13 * 32, 32, 64, "PLAYER"));
	player->setCurrentRow(0);
	player->setCurrentFrame(0);
	gameObjects.push_back(player);
	return true;
}

bool PlayingState::onExit() {
	for (auto obj : gameObjects) {
		obj->clean();
	}
	gameObjects.clear();
#ifdef __DEBUG
	std::cerr << "Playing state - EXIT" << std::endl;
#endif

	return true;
}