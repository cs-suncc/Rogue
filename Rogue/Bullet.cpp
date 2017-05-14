#include "Bullet.h"
#include "Game.h"

void Bullet::draw() {
	TextureManager::Instance().drawFrame(textureID, x, y, width, height, currentRow, currentFrame, Game::Instance().getRenderer());
}
