#include "gameover.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "Game.h"
#include "SDLGameObject.h"
#include "AudioManager.h"

void GameOverState::update() {
	InputHandler::Instance().update();
	if (InputHandler::Instance().buttonState(0, XBoxInputNodes::BUTTON_A)) {
		throw std::exception("mainmenu");
	}
}

void GameOverState::render() {
	for (auto obj : gameObjects) {
		obj->draw();
	}
}

bool GameOverState::onEnter()
{
	TextureManager::Instance().load("asset/image/GameOver.png", "GAMEOVER", Game::Instance().getRenderer());
	Mix_PauseMusic();
	gameObjects.push_back(new SDLGameObject());
	gameObjects[0]->load(LoaderParams(0, 0, 960, 640, "GAMEOVER"));
	return true;
}

bool GameOverState::onExit()
{
	return true;
}


