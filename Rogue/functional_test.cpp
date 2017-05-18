#include "SDL.h"
#include "Game.h"
#include "InputHandler.h"
#include <iostream>
#include "MapLoader.h"
#define ROGUE_SHOWFPS false
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

void functional_test_state_machine() {
	Game::Instance().init("UI Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 640);
	auto tick = 0;
	auto fpsCounter = 0;
	while (Game::Instance().running()) {
		if (SDL_GetTicks() - tick < 30) {
			continue;
		}
		tick = SDL_GetTicks();
		fpsCounter = tick;
		Game::Instance().update();
		Game::Instance().render();
		fpsCounter = SDL_GetTicks() - fpsCounter;

//#ifdef _DEBUG
		if(ROGUE_SHOWFPS)
			std::cerr << "Max FPS:" << 1000.0 / fpsCounter << std::endl;
//#endif
	}
	//Game::Instance().quit();
}