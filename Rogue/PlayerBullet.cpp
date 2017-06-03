#include "Bullet.h"
#include "Game.h"
#include "AudioManager.h"
#include "MapManager.h"
#include "playing.h"
#include <cmath>
void PlayerMagicBullet::playHitSound() {
	AudioManager::Instance().playSound("MAGICHIT");
}

void PlayerMagicBullet::update() {
	livetime++;
	velocity = Vector2(velocity.getX(), 7 * cos(double(livetime) / 6 * M_PI));
	auto _state = static_cast<PlayingState *>(Game::Instance().getGameStateMachine()->currentState());
	auto mp = MapManager::Instance().getMap(_state->getCurrentMap());
	if (!mp->getTile((y + 5) / 32, (_state->getViewportLeft() + x + 5) / 32)->passable())
		destroy = true;
	if (x < -200 || x > 2000) {
		destroy = true;
	}
	Bullet::update();
}

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

void PlayerBullet::update()
{
	auto _state = static_cast<PlayingState *>(Game::Instance().getGameStateMachine()->currentState());
	auto mp = MapManager::Instance().getMap(_state->getCurrentMap());
	if (!mp->getTile((y + 5) / 32, (_state->getViewportLeft() + x + 5) / 32)->passable())
		destroy = true;
	if (livetime++ > 10) {
		destroy = true;
	}
	Bullet::update();
}

void PlayerBullet::draw() {
	TextureManager::Instance().drawFrame(textureID, x, y, width, height, currentRow, currentFrame, Game::Instance().getRenderer(),
		_flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void PlayerBullet::playHitSound() {
	AudioManager::Instance().playSound("ATTACKHIT");
}
