#include "Enemy.h"
#include "AudioManager.h"
EnemyZombie::EnemyZombie()
{
	load(LoaderParams(0, 0, 40, 50, "ENEMYZOMBIE"));
	currentRow = 0;
	currentFrame = 0;
	next_moving_frame = 125 + rand() % 50;
	faceflip = rand() % 2;
}

SDL_Rect EnemyZombie::getBox()
{
	if (currentState == DYING)
		return { -1, -1, 1, 1 };
	if (currentFrame == 0)
		return { x + 4, y + 3, 26, 47 };
	if (currentFrame == 1)
		return { x + 3, y + 3, 32, 46 };
	if (currentFrame == 2)
		return { x + 2, y + 5, 26, 47 };
	return { x + 3, y + 5, 32, 46 };
}

void EnemyZombie::update()
{
	if (hitpoint <= 0 && currentState != DYING) {
		currentState = DYING;
		dying = false;
		AudioManager::Instance().playSound("ENEMYDESTROY");
		textureID = "BULLET";
		currentFrame = 0;
		currentRow = 2;
		width = 32;
		height = 32;
		stalling_frame = 0;
	}
	switch (currentState) {
	case STALL:
		if (stalling_frame++ == 76) {
			velocity.setX((faceflip ? -1 : 1) * 1);
			faceflip = !faceflip;
			currentState = MOVING;
			moving_frame = 0;
			next_moving_frame = rand() % 100 + 50;
		}
		break;
	case MOVING:
		if (moving_frame++ == next_moving_frame) {
			velocity.setX(0);
			currentState = STALL;
			stalling_frame = 0;
		}
		break;
	case DYING:
		if (++stalling_frame == 20)
			dying = true;
		return;
	}
	velocity += accelerate;
	x += (int)(velocity.getX());
	y += (int)(velocity.getY());
	if (next_frame++ == 20) {
		next_frame = 0;
		currentFrame = (currentFrame + 1) % 4;
	}
}
