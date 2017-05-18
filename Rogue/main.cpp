#include <iostream>
#include "SDL.h"
#include "Game.h"

extern void functional_test_sdl();
extern void functional_test_Game();
extern void functional_test_state_machine();

int main(int argc, char **args) {
	//functional_test_state_machine();
	
	Game::Instance().init("Rouge", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 640);
	auto tick = 0;
	while (Game::Instance().running()) {
		if (SDL_GetTicks() - tick < 30) {
			continue;
		}
		tick = SDL_GetTicks();
		Game::Instance().update();
		Game::Instance().render();
	}
	return 0;
}