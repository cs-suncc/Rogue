#include "SDLGameObject.h"
#include "TextureManager.h"
#include "Game.h"

SDLGameObject::SDLGameObject() :
	GameObject() {
}

void SDLGameObject::load(const LoaderParams &param) {
	x = param.getX();
	y = param.getY();
	width = param.getWidth();
	height = param.getHeight();
	textureID = param.getTextureID();
	currentRow = 0;
	currentFrame = 0;
}

void SDLGameObject::draw() {
	TextureManager::Instance().drawFrame(textureID, x, y, width, height, currentRow, currentFrame, Game::Instance().getRenderer());
}

void SDLGameObject::update() {
}

void SDLGameObject::clean() {
}

void SDLGameObject::setCurrentRow(int v) {
	currentRow = v;
}

void SDLGameObject::setCurrentFrame(int v) {
	currentFrame = v;
}
