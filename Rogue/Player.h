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
	int getHitpoint() {
		return hitpoint;
	}
	int getHitpointMax() {
		return hitpointmax;
	}
	void setHitpoint(int hp) {
		hitpoint = hp;
	}
	int getMana() {
		return mana;
	}
	int getManaMax() {
		return ManaMax;
	}
	void setMana(int mp) {
		mana = mp;
	}
	bool immutable() {
		return immute;
	}
	void setImmutable() {
		immute = true;
		immutable_countdown = 20;
	}
	bool isDefending() {
		return currentState == DEFENDING;
	}
	void setX(int _x) {
		x = _x;
	}
private:
	enum PlayerState {
		MOVING,
		JUMPING,
		ATTACKING,
		SPELLING,
		DEFENDING
	};
	bool faceflip = false;
	int hitpoint = 100;
	int hitpointmax = 100;
	int ManaMax = 200;
	int mana = 200;
	bool immute = false;
	bool display = true;
	int immutable_countdown = 0;
	Vector2 velocity;
	Vector2 accelerate;
	PlayerState currentState = MOVING;
};

class PlayerCreator: public BaseCreator {
public:
	GameObject *createGameObject() const override {
		return new Player();
	}
};