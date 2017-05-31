#include "Enemy.h"

Boss::Boss() {
	load(LoaderParams(440, 156, 80, 96, "BOSS"));
	currentRow = 0;
	currentFrame = 0;
	next_frame = 0;
	hitpoint_max = 5000;
	hitpoint = 5000;
}

void Boss::update() {
	if (next_frame++ == 5) {
		currentFrame = (currentFrame + 1) % 4;
		next_frame = 0;
	}
}