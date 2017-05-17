#pragma once
#include "SDLGameObject.h"
#include "GameObjectFactory.h"
#include "TextureManager.h"
#include "Vector2.h"
#include "Game.h"

class Enemy : public SDLGameObject {
public:
	Enemy() 
		:velocity(0, 0), accelerate(0, 0){
	}
	virtual SDL_Rect getBox() = 0; //»ñÈ¡Åö×²ºÐ
	void flip() {
		faceflip = !faceflip;
	}

	virtual void update() override {
		currentFrame = (currentFrame + 1) % 4;
	}
	virtual void draw() override {
		TextureManager::Instance().drawFrame(textureID, x, y, width, height, currentRow, currentFrame, Game::Instance().getRenderer(),
			faceflip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
	}

	int getHitpoint() {
		return hitpoint;
	}
	void setHitpoint(int hp) {
		hitpoint = hp;
	}
	int getMaxHitpoint() {
		return hitpoint_max;
	}
	void setMaxHitpoint(int max_hp) {
		hitpoint_max = max_hp;
	}
	void setX(int _x) {
		x = _x;
	}
	void setY(int _y) {
		y = _y;
	}
	bool perish() {
		return dying;
	}
protected:
	int hitpoint = 0;
	int hitpoint_max = 0;
	bool faceflip = false;
	bool dying = false;
	Vector2 velocity;
	Vector2 accelerate;
};

class EnemyBat : public Enemy {
public:
	EnemyBat();
	virtual SDL_Rect getBox();
	virtual void update() override;
private:
	enum BatState {
		STALL,
		MOVING_LEFT,
		MOVING_RIGHT,
		DYING
	};
	int stalling_frame = 0;
	int moving_frame = 0;
	int next_frame = 0;
	BatState currentState = STALL;
};

class EnemyBatCreator : public BaseCreator {
public:
	virtual GameObject *createGameObject() const override {
		return new EnemyBat();
	}
};