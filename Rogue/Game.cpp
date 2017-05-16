#include "Game.h"
#include "mainmenu.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "AudioManager.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include <iostream>
#include <exception>
#include <cmath>

bool Game::init(const char * title, int x, int y, int width, int height, int flags) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "[Info] SDL Init Success" << std::endl;
		//初始化游戏窗口和renderer
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
	
	gof.registerType("Player", new PlayerCreator());
	gof.registerType("PlayerBullet", new PlayerBulletCreator());
	gof.registerType("PlayerMagicBullet", new PlayerMagicBulletCreator());
	gof.registerType("EnemyBat", new EnemyBatCreator());

	gameStateMachine = new StateMachine();
	gameStateMachine->pushState(new MainMenuState());
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
	gameStateMachine->render();
	SDL_RenderPresent(renderer);
}

void Game::update() {
	gameStateMachine->update();
}

void Game::clean() {
}

void Game::loadAssets() {
}

static bool inRect(int x, int y, SDL_Rect r) {
	return x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h;
}

bool collision(SDL_Rect r1, SDL_Rect r2)
{
	bool inside =
		inRect(r1.x, r1.y, r2) || inRect(r1.x + r1.w, r1.y, r2) ||
		inRect(r1.x, r1.y + r1.h, r2) || inRect(r1.x + r1.w, r1.y + r1.h, r2) ||
		inRect(r2.x, r2.y, r1) || inRect(r2.x + r2.w, r2.y, r1) ||
		inRect(r2.x, r2.y + r2.h, r1) || inRect(r2.x + r2.w, r2.y + r1.h, r1);
	if (inside)
		return true;
	//through
	Vector2 x1(r1.x, r1.y + 0.5*r1.h), x2(r1.x + r1.w, r1.y + 0.5*r1.h);
	Vector2 y1(r2.x + 0.5*r2.w, r2.y), y2(r2.x + 0.5*r2.w, r2.y + r2.h);
	return
		(x2 - x1).dot(y1 - x1) * (x2 - x1).dot(y2 - x1) < 0 &&
		(y2 - y1).dot(x1 - y1) * (y2 - y1).dot(x2 - y1) < 0;
}
