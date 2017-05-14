#pragma once
#include "SDLGameObject.h"
#include "Vector2.h"
#include "LoaderParams.h"
#include "GameObjectFactory.h"
#include "TextureManager.h"

class Bullet : public SDLGameObject {
public:
	Bullet()
		:velocity(0, 0), accelerate(0, 0) {
		load(LoaderParams(0, 0, 32, 32, "BULLET"));
	}
	virtual void load(const LoaderParams &param) override {
		SDLGameObject::load(param);
	}
	virtual void draw() override;
	virtual void update() override {
		velocity += accelerate;
		x += (int)(velocity.getX());
		y += (int)(velocity.getY());
		currentFrame = (currentFrame + 1) % 4;
	}
	virtual void clean() override {
	}
	void setVelocity(Vector2 v) {
		velocity = v;
	}
	const Vector2 &getVelocity() {
		return const_cast<const Vector2 &>(velocity);
	}
	void setAccelerate(Vector2 a) {
		accelerate = a;
	}
	const Vector2 &getAccelerate() {
		return const_cast<const Vector2 &>(accelerate);
	}
	void setX(int _x) {
		x = _x;
	}
	void setY(int _y) {
		y = _y;
	}
	int getX() {
		return x;
	}
	int getY() {
		return y;
	}
private:
	Vector2 velocity;
	Vector2 accelerate;
};

class PlayerMagicBullet : public Bullet {
public:
	PlayerMagicBullet() {
		load(LoaderParams(0, 0, 32, 32, "BULLET"));
	}
	virtual void load(const LoaderParams &param) override;
};

class PlayerMagicBulletCreator : public BaseCreator {
public:
	GameObject *createGameObject() const override {
		return new PlayerMagicBullet();
	}
};

class PlayerBullet : public Bullet {
public:
	PlayerBullet() {
		load(LoaderParams(0, 0, 32, 32, "BULLET"));
	}
	virtual void load(const LoaderParams &param) override;
	void flip() {
		_flip = !_flip;
	}
	virtual void draw() override;
private:
	bool _flip = false;
};

class PlayerBulletCreator : public BaseCreator {
public:
	GameObject *createGameObject() const override {
		return new PlayerBullet();
	}
};