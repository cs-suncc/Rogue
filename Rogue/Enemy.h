#pragma once
#include "SDLGameObject.h"
#include "GameObjectFactory.h"
#include "TextureManager.h"
#include "Vector2.h"
#include "Game.h"

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG


class Enemy : public SDLGameObject {
public:
	Enemy() 
		:velocity(0, 0), accelerate(0, 0){
	}
	virtual SDL_Rect getBox() = 0; //»ñÈ¡Åö×²ºÐ
	virtual int getDamage() = 0;
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
	virtual std::string getType() = 0;
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
	virtual SDL_Rect getBox() override;
	virtual int getDamage() {
		return 20;
	}
	virtual void update() override;
	virtual std::string getType() override {
		return "EnemyBat";
	}
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
	int next_moving_frame = 0;
	BatState currentState = STALL;
};

class EnemyBatCreator : public BaseCreator {
public:
	virtual GameObject *createGameObject() const override {
		return new EnemyBat();
	}
};

class BossBat : public Enemy {
public:
	BossBat();
	virtual SDL_Rect getBox() override;
	virtual int getDamage() {
		return 25;
	}
	virtual void update() override;
	virtual std::string getType() override {
		return "BossBat";
	}
private:
	enum BatState {
		STALL,
		MOVING_LEFT,
		MOVING_RIGHT,
		DYING
	};
	int stalling_frame = 0;
	int next_frame = 0;
	BatState currentState = STALL;
};

class BossBatCreator : public BaseCreator {
public:
	virtual GameObject *createGameObject() const override {
		return new BossBat();
	}
};

class EnemyZombie : public Enemy {
public:
	EnemyZombie();
	virtual SDL_Rect getBox() override;
	virtual int getDamage() {
		return 25;
	}
	virtual void update() override;
	virtual std::string getType() {
		return "EnemyZombie";
	}
private:
	enum ZombieState {
		STALL,
		MOVING,
		DYING
	};
	int stalling_frame = 0;
	int moving_frame = 0;
	int next_frame = 0;
	int next_moving_frame = 0;
	ZombieState currentState = STALL;
};

class EnemyZombieCreator : public BaseCreator {
public:
	virtual GameObject *createGameObject() const override {
		return new EnemyZombie();
	}
};

class Boss : public Enemy {
public:
	Boss();
	virtual SDL_Rect getBox() override {
		return { x, y, 77, 84 };
	}
	virtual int getDamage() {
		return 40;
	}
	virtual void update() override;
	virtual std::string getType() {
		return "Boss";
	}
private:
	enum BossState {
		STALL,
		SUMMONZOMBIE,
		SUMMONBAT,
		FIRE,
		ICE,
		DYING
	};
	int next_frame = 0;
	int counter;
	BossState currentState = STALL;
};

class BossCreator : public BaseCreator {
public:
	virtual GameObject *createGameObject() const override {
		return new Boss();
	}
};