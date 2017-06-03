#include "pause.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "Game.h"
#include "SDLGameObject.h"
#include "AudioManager.h"
#include <algorithm>

void PauseState::update() {
	static auto nextreact = true;
	InputHandler::Instance().update();
	if (InputHandler::Instance().yValue(0, 1) == 0 && !InputHandler::Instance().buttonState(0, XBoxInputNodes::BUTTON_A)) {
		nextreact = true;
	}
	if (nextreact && InputHandler::Instance().yValue(0, 1) > 0) {
		buttons[selected]->setCurrentFrame(0);
		selected = 1;
		buttons[selected]->setCurrentFrame(1);
		AudioManager::Instance().playSound("UIEFFECT");
		nextreact = false;
	}
	if (nextreact && InputHandler::Instance().yValue(0, 1) < 0) {
		buttons[selected]->setCurrentFrame(0);
		selected = 0;
		buttons[selected]->setCurrentFrame(1);
		AudioManager::Instance().playSound("UIEFFECT");
		nextreact = false;
	}
	if (nextreact && InputHandler::Instance().buttonState(0, XBoxInputNodes::BUTTON_A)) {
		AudioManager::Instance().playSound("UIEFFECT");
		nextreact = false;
		switch (selected) {
		case 0:
			throw std::exception("resume");
		case 1:
			throw std::exception("mainmenu");
		}
	}
}

void PauseState::render() {
	for (auto obj : gameObjects) {
		obj->draw();
	}
}

bool PauseState::onEnter() {
	TextureManager::Instance().load("asset/image/pause.png", "PAUSE", Game::Instance().getRenderer());
	TextureManager::Instance().load("asset/image/PauseButton.png", "PAUSEBUTTON", Game::Instance().getRenderer());
	AudioManager::Instance().loadSound("asset/audio/middle.mp3", "UIEFFECT");
	auto title = new SDLGameObject();
	title->load(LoaderParams(0, 0, 960, 640, "PAUSE"));
	gameObjects.push_back(title);

	for (int i = 0; i < 2; ++i) {
		auto p = new SDLGameObject();
		p->load(LoaderParams(360, 394 + i * 32, 240, 32, "PAUSEBUTTON"));
		p->setCurrentFrame(0);
		p->setCurrentRow(i);
		buttons.push_back(p);
		gameObjects.push_back(p);
	}
	buttons[selected]->setCurrentFrame(1);
	return true;
}

bool PauseState::onExit() {
	for (auto obj : gameObjects) {
		obj->clean();
	}
	gameObjects.clear();
#ifdef _DEBUG
	std::cerr << "Pause state - EXIT" << std::endl;
#endif
	return true;
}
