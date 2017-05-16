#include <iostream>
#include "playing.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "Game.h"
#include "SDLGameObject.h"
#include "Player.h"
#include "GameObjectFactory.h"
#include "MapManager.h"
#include "AudioManager.h"
#include "UI.h"

const std::string PlayingState::playingID = "PLAYING";

void PlayingState::update() {
	InputHandler::Instance().update();
	if (InputHandler::Instance().buttonState(0, XBoxInputNodes::BUTTON_BACK)) {
		Game::Instance().getGameStateMachine()->popState();
		AudioManager::Instance().playMusic("TITLEBGM");
		return;
	}
	for (auto obj : gameObjects) {
		obj->update();
	}
	for (auto blt : bullets) {
		blt->update();
	}
}

void PlayingState::render() {
	SDL_SetRenderDrawColor(Game::Instance().getRenderer(), 227, 227, 227, 255);
	SDL_RenderClear(Game::Instance().getRenderer());
	MapManager::Instance().getMap(currentMap)->draw();
	for (auto obj : gameObjects) {
		obj->draw();
	}
	for (auto blt : bullets) {
		blt->draw();
	}
	UI::Instance().draw();
}

bool PlayingState::onEnter() {
	MapManager::Instance().loadMap("TESTMAP", "asset/test.tmx");
	currentMap = "TESTMAP";
	TextureManager::Instance().load("asset/image/rogue2.png", "PLAYER", Game::Instance().getRenderer());
	TextureManager::Instance().load("asset/image/bullet.png", "BULLET", Game::Instance().getRenderer());
	AudioManager::Instance().loadMusic("asset/audio/dungeon.mp3", "PLAYINGBGM");
	AudioManager::Instance().loadSound("asset/audio/PlayerAttack.mp3", "PLAYERATTACK");
	AudioManager::Instance().loadSound("asset/audio/PlayerMagic.mp3", "PLAYERMAGIC");
	AudioManager::Instance().loadSound("asset/audio/PlayerDefending.mp3", "PLAYERDEFENDING");

	SDL_SetRenderDrawColor(Game::Instance().getRenderer(), 227, 227, 227, 255);

	auto *player = (Player *)Game::Instance().factories().create("Player");
	player->load(LoaderParams(0 * 32, 13 * 32, 32, 64, "PLAYER"));
	player->setCurrentRow(0);
	player->setCurrentFrame(0);
	gameObjects.push_back(player);
	AudioManager::Instance().playMusic("PLAYINGBGM");
	return true;
}

bool PlayingState::onExit() {
	for (auto obj : gameObjects) {
		obj->clean();
	}
	gameObjects.clear();
#ifdef _DEBUG
	std::cerr << "Playing state - EXIT" << std::endl;
#endif

	return true;
}

void PlayingState::addBullet(Bullet * blt) {
	bullets.push_back(blt);
}
