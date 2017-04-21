#pragma once
#include "SDLGameObject.h"
#include "LoaderParams.h"
#include "GameObjectFactory.h"

class Player: public SDLGameObject {
public:
	Player();
	void load(const LoaderParams &paras) override;
	void draw() override;
	void update() override;
	void clean() override;
};

class PlayerCreator: public BaseCreator {
public:
	GameObject *createGameObject() const override {
		return new Player();
	}
};