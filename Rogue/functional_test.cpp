#include "SDL.h"
#include "Game.h"
#include "InputHandler.h"
#include <iostream>
#include "MapLoader.h"

void functional_test_sdl() {
	SDL_Init(SDL_INIT_EVERYTHING);
	auto wd = SDL_CreateWindow("SDL Functional test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
	auto rd = SDL_CreateRenderer(wd, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(rd, 0xaa, 0xaa, 0x00, 0xff);
	SDL_RenderClear(rd);
	SDL_RenderPresent(rd);
	SDL_Delay(1000l);
	SDL_Quit();
}

void functional_test_Game() {
	Game::Instance().init("Game Funcional test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480);
	while (Game::Instance().running()) {
		Game::Instance().handleEvents();
		if (InputHandler::Instance().buttonState(0, XBoxInputNodes::BUTTON_START))
			Game::Instance().quit();
	}
}

/*void functional_test_input() {
	Game::Instance().init("Input test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 640);
	while (Game::Instance().running()) {
		if (InputHandler::Instance().buttonState(0, XBoxInputNodes::BUTTON_START))
			Game::Instance().quit();
		std::cout << InputHandler::Instance().xValue() << std::endl;
	}
}*/

void functional_test_MapLoader() {
	Game::Instance().init("MapLoaderTest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 640);
	Map mp("asset/test.tmx");
	SDL_SetRenderDrawColor(Game::Instance().getRenderer(), 227, 227, 227, 255);
	while (Game::Instance().running()) {
		SDL_Delay(33);
		Game::Instance().handleEvents();
		if (InputHandler::Instance().buttonState(0, XBoxInputNodes::BUTTON_START))
			Game::Instance().quit();
		Game::Instance().update();
		int w = mp.width;
		int h = mp.height;
		for (int i = 0; i < h; ++i)
			for (int j = 0; j < w; ++j) {
				mp._tile->draw(mp.layer[i][j], 32 * j, 32 * i);
			}
		Game::Instance().render();
	}
}

void functional_test_state_machine() {
	Game::Instance().init("MapLoaderTest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 640);
	auto tick = 0;
	while (Game::Instance().running()) {
		if (SDL_GetTicks() - tick < 34) {
			continue;
		}
		tick = SDL_GetTicks();
		Game::Instance().update();
		Game::Instance().render();
	}
	//Game::Instance().quit();
}