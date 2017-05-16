#include <iostream>
#include "mainmenu.h"
#include "playing.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "Game.h"
#include "SDLGameObject.h"
#include "AudioManager.h"

const std::string MainMenuState::menuID = "MAINMENU";

void MainMenuState::update() {
	InputHandler::Instance().update();
	if (InputHandler::Instance().buttonState(0, XBoxInputNodes::BUTTON_START)) {
		Game::Instance().getGameStateMachine()->pushState(new PlayingState());
	}
	if (InputHandler::Instance().buttonState(0, XBoxInputNodes::BUTTON_A)) {
		onExit();
		Game::Instance().quit();
	}
}

void MainMenuState::render() {
	for (auto obj : gameObjects) {
		obj->draw();
	}
}

bool MainMenuState::onEnter() {
	auto f = TextureManager::Instance().load("asset/image/title.png", "TITLE", Game::Instance().getRenderer());
	AudioManager::Instance().loadMusic("asset/audio/Title.mp3", "TITLEBGM");
	if (!f) {
		return false;
	}
	auto p = new SDLGameObject();
	p->load(LoaderParams(0, 0, 960, 640, "TITLE"));
	gameObjects.push_back(p);
	AudioManager::Instance().playMusic("TITLEBGM");
	return true;
}

bool MainMenuState::onExit() {
	for (auto obj : gameObjects) {
		obj->clean();
	}
	gameObjects.clear();
	TextureManager::Instance().unload("TITLE");
#ifdef _DEBUG
	std::cerr << "Main menu state - EXIT" << std::endl;
#endif
	return true;
}
