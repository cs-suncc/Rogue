#include "win.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "Game.h"
#include "SDLGameObject.h"
#include "AudioManager.h"

void WinState::update() {
	InputHandler::Instance().update();
	if (InputHandler::Instance().buttonState(0, XBoxInputNodes::BUTTON_A)) {
		throw std::exception("mainmenu");
	}
}

void WinState::render() {
	for (auto obj : gameObjects) {
		obj->draw();
	}
}

bool WinState::onEnter()
{
	TextureManager::Instance().load("asset/image/win.png", "WIN", Game::Instance().getRenderer());
	AudioManager::Instance().loadMusic("asset/audio/win.mp3", "WIN");
	AudioManager::Instance().playMusic("WIN");
	gameObjects.push_back(new SDLGameObject());
	gameObjects[0]->load(LoaderParams(0, 0, 960, 640, "WIN"));
	return true;
}

bool WinState::onExit()
{
	return true;
}


