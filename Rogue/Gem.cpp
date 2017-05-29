#include "Gem.h"

SDL_Rect Gem::getBox() {
	if (currentState != READY) {
		return { -1, -1, 1, 1 };
	} else {
		return { x + 5, y + 5, 22, 22 };
	}
}

void Gem::update() {
	switch (currentState) {
	case READY:
		if (skip++ < 3)
			break;
		if (currentRow % 2 == 0) {
			++currentRow;
		} else {
			--currentRow;
		}
		skip = 0;
		break;
	case EMPTY:
		if (empty_time++ >= 150) {
			currentState = REGENERATE;
			currentFrame = 1;
			break;
		}
		if (skip++ < 3)
			break;
		if (currentRow % 2 == 0) {
			++currentRow;
		} else {
			--currentRow;
		}
		skip = 0;
		break;
	case REGENERATE:
		if (skip++ < 3)
			break;
		currentState = READY;
		currentFrame = 0;
		skip = 0;
		break;
	}
}

void Gem::activate() {
	currentState = EMPTY;
	empty_time = 0;
	skip = 0;
	currentFrame = 2;
}
