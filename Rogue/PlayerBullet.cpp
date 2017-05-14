#include "Bullet.h"
#include "Game.h"

void PlayerMagicBullet::load(const LoaderParams & param) {
	x = param.getX();
	y = param.getY();
	width = param.getWidth();
	height = param.getHeight();
	textureID = param.getTextureID();
	currentRow = 0;
	currentFrame = 0;
}

void PlayerBullet::load(const LoaderParams & param) {
	x = param.getX();
	y = param.getY();
	width = param.getWidth();
	height = param.getHeight();
	textureID = param.getTextureID();
	currentRow = 1;
	currentFrame = 0;
}

void PlayerBullet::draw() {
	TextureManager::Instance().drawFrame(textureID, x, y, width, height, currentRow, currentFrame, Game::Instance().getRenderer(),
		_flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}
