#pragma once
#include "SDLGameObject.h"
#include "GameObjectFactory.h"
#include "TextureManager.h"
#include "Vector2.h"
#include "Game.h"

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG


class Gem : public SDLGameObject {
public:
	Gem() {
		textureID = "GEM";
		width = 32;
		height = 32;
	}
	void setX(int _x) {
		x = _x;
	}
	void setY(int _y) {
		y = _y;
	}
	virtual SDL_Rect getBox();
	virtual std::string getType() = 0;
	virtual void update() override;
	void activate();

protected:
	enum GemState {
		READY,
		EMPTY,
		REGENERATE
	};
	GemState currentState = READY;
	int empty_time = 0;
	int skip = 0;
};

class HealGem : public Gem {
public:
	HealGem() {
		currentFrame = 0;
		currentRow = 0;
	}
	virtual std::string getType() override{
		return "HealGem";
	}
};

class HealGemCreator : public BaseCreator {
public:
	virtual GameObject *createGameObject() const override {
		return new HealGem();
	}
};

class ManaGem : public Gem {
public:
	ManaGem() {
		currentFrame = 0;
		currentRow = 2;
	}
	virtual std::string getType() override {
		return "ManaGem";
	}
};

class ManaGemCreator : public BaseCreator {
public:
	virtual GameObject *createGameObject() const override {
		return new ManaGem();
	}
};