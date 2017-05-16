#pragma once
#include "SDLGameObject.h"
#include "LoaderParams.h"
#include "GameObjectFactory.h"
#include "Vector2.h"
class Player: public SDLGameObject {
public:
	Player();
	void load(const LoaderParams &paras) override;
	void draw() override;
	void update() override;
	void clean() override;
private:
	bool faceflip = false;
	int hitpoint = 100;
	int ManaMax = 200;
	int mana = 200;
	Vector2 velocity;
	Vector2 accelerate;

};

class PlayerCreator: public BaseCreator {
public:
	GameObject *createGameObject() const override {
		return new Player();
	}
};