#include <iostream>
#include "mainmenu.h"
#include "playing.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "Game.h"
#include "SDLGameObject.h"
#include "AudioManager.h"
#include <algorithm>

const std::string MainMenuState::menuID = "MAINMENU";

void MainMenuState::update() {
	static auto nextreact = true;
	InputHandler::Instance().update();
	if (InputHandler::Instance().yValue(0, 1) == 0 && !InputHandler::Instance().buttonState(0, XBoxInputNodes::BUTTON_A)) {
		nextreact = true;
	}
	if (nextreact && InputHandler::Instance().yValue(0, 1) > 0) {
		mainmenubuttons[selected]->setCurrentFrame(0);
		selected = std::min(selected + 1, 2);
		mainmenubuttons[selected]->setCurrentFrame(1);
		AudioManager::Instance().playSound("UIEFFECT");
		nextreact = false;
	}
	if (nextreact && InputHandler::Instance().yValue(0, 1) < 0) {
		mainmenubuttons[selected]->setCurrentFrame(0);
		selected = std::max(selected - 1, 0);
		mainmenubuttons[selected]->setCurrentFrame(1);
		AudioManager::Instance().playSound("UIEFFECT");
		nextreact = false;
	}
	if (nextreact && InputHandler::Instance().buttonState(0, XBoxInputNodes::BUTTON_A)) {
		AudioManager::Instance().playSound("UIEFFECT");
		nextreact = false;
		switch(selected) {
			case 0:
				gameObjects[0] = title;
				Game::Instance().getGameStateMachine()->pushState(new PlayingState());
				break;
			case 1:
				if (gameObjects[0] == title)
					gameObjects[0] = help;
				else
					gameObjects[0] = title;
				break;
			case 2:
				onExit();
				Game::Instance().quit();
		}
	}
}

void MainMenuState::render() {
	for (auto obj : gameObjects) {
		obj->draw();
	}
}

bool MainMenuState::onEnter() {
	TextureManager::Instance().load("asset/image/title.png", "TITLE", Game::Instance().getRenderer());
	TextureManager::Instance().load("asset/image/help.png", "HELP", Game::Instance().getRenderer());
	TextureManager::Instance().load("asset/image/MainButton.png", "MAINBUTTON", Game::Instance().getRenderer());
	AudioManager::Instance().loadSound("asset/audio/middle.mp3", "UIEFFECT");
	AudioManager::Instance().loadMusic("asset/audio/Title.mp3", "TITLEBGM");
	title = new SDLGameObject();
	title->load(LoaderParams(0, 0, 960, 640, "TITLE"));
	gameObjects.push_back(title);
	help = new SDLGameObject();
	help->load(LoaderParams(0, 0, 960, 640, "HELP"));

	for (int i = 0; i < 3; ++i) {
		auto p = new SDLGameObject();
		p->load(LoaderParams(530, 394 + i * 32, 240, 32, "MAINBUTTON"));
		p->setCurrentFrame(0);
		p->setCurrentRow(i);
		mainmenubuttons.push_back(p);
		gameObjects.push_back(p);
	}
	mainmenubuttons[selected]->setCurrentFrame(1);
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
