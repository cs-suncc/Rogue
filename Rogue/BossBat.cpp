#include "Enemy.h"
#include "AudioManager.h"
#ifdef _DEBUG
#include <iostream>
#endif

BossBat::BossBat() {
	load(LoaderParams(0, 0, 32, 34, "ENEMYBAT"));
	currentRow = 0;
	currentFrame = 0;
}

SDL_Rect BossBat::getBox()
{
	if (currentState == DYING)
		return { -1, -1, 1, 1 };
	if (currentFrame == 0)
		return { x + 4, y, 26, 24 };
	if (currentFrame == 1)
		return { x + 2, y + 5, 28, 21 };
	if (currentFrame == 2)
		return { x + 3, y + 4, 19, 28 };
	return { x + 3, y + 5, 27, 21 };
}

void BossBat::update()
{
	if ((hitpoint <= 0 || x < -32 || x > 960) && currentState != DYING) {
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
		if (stalling_frame++ == 30) {
			velocity.setX((faceflip ? 1 : -1) * 1);
			currentState = faceflip ? MOVING_RIGHT : MOVING_LEFT;
		}
		break;
	case MOVING_LEFT:
		break;
	case MOVING_RIGHT:
		break;
	case DYING:
		if (++stalling_frame == 20)
			dying = true;
		return;
	}
	velocity += accelerate;
	x += (int)(velocity.getX());
	y += (int)(velocity.getY());
	if (next_frame++ == 5) {
		next_frame = 0;
		currentFrame = (currentFrame + 1) % 4;
	}
#ifdef _DEBUG
	//std::cerr << "Bat<" << this << "> x:" << x << " y:" << y << std::endl;
#endif // _DEBUG

}
