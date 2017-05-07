#include <iostream>
#include "SDL.h"

extern void functional_test_sdl();
extern void functional_test_Game();
extern void functional_test_state_machine();

int main(int argc, char **args) {
	functional_test_state_machine();
	return 0;
}