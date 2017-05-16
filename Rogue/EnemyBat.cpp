#include "Enemy.h"

enum BatState {
	STALL,
	MOVING_LEFT,
	MOVING_RIGHT,
	DYING
};

EnemyBat::EnemyBat() {
	load(LoaderParams(0, 0, 32, 34, "ENEMYBAT"));
	currentRow = 0;
	currentFrame = 0;
}

SDL_Rect EnemyBat::getBox()
{
	if (currentFrame == 0)
		return { x + 4, y, 26, 24 };
	if (currentFrame == 1)
		return { x + 2, y + 5, 28, 21 };
	if (currentFrame == 2)
		return { x + 3, y + 4, 19, 28 };
	return { x + 3, y + 5, 27, 21 };
}

void EnemyBat::update()
{
	static auto stalling_frame = 0;
	static auto moving_frame = 0;
	static auto next_frame = 0;
	static auto currentState = STALL;
	if (dying) {
		currentState = DYING;
	}
	switch (currentState) {
	case STALL:
		if (stalling_frame++ == 76) {
			velocity.setX((faceflip ? -1 : 1) * 1);
			faceflip = !faceflip;
			currentState = faceflip ? MOVING_RIGHT : MOVING_LEFT;
			moving_frame = 0;
		}
		break;
	case MOVING_LEFT:
		if (moving_frame++ == 150) {
			velocity.setX(0);
			currentState = STALL;
			stalling_frame = 0;
		}
		break;
	case MOVING_RIGHT:
		if (moving_frame++ == 150) {
			velocity.setX(0);
			currentState = STALL;
			stalling_frame = 0;
		}
		break;
	case DYING:
		// TODO
		break;
	}
	velocity += accelerate;
	x += (int)(velocity.getX());
	y += (int)(velocity.getY());
	if (next_frame++ == 5) {
		next_frame = 0;
		currentFrame = (currentFrame + 1) % 4;
	}
	
}
