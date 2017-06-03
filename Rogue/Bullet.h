#pragma once
#include "SDLGameObject.h"
#include "Vector2.h"
#include "LoaderParams.h"
#include "GameObjectFactory.h"
#include "TextureManager.h"
#include <functional>

class Bullet : public SDLGameObject {
public:
	Bullet()
		:velocity(0, 0), accelerate(0, 0) {
		load(LoaderParams(0, 0, 32, 32, "BULLET"));
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
	int getDamage() {
		return damage;
	}
	virtual void playHitSound() = 0;
	bool perish() {
		return destroy;
	}
protected:
	Vector2 velocity;
	Vector2 accelerate;
	int damage = 0;
	bool destroy = false;
	int livetime = 0;
};

class PlayerMagicBullet : public Bullet {
public:
	PlayerMagicBullet() {
		load(LoaderParams(0, 0, 32, 32, "BULLET"));
		damage = 100;
	}
	void playHitSound() override;
	virtual void update() override;
	virtual void load(const LoaderParams &param) override;
};

/*class PlayerMagicBulletCreator : public BaseCreator {
public:
	GameObject *createGameObject() const override {
		return new PlayerMagicBullet();
	}
};*/

class PlayerBullet : public Bullet {
public:
	PlayerBullet() {
		load(LoaderParams(0, 0, 32, 32, "BULLET"));
		damage = 15;
	}
	virtual void load(const LoaderParams &param) override;
	void flip() {
		_flip = !_flip;
	}
	virtual void update() override;
	virtual void draw() override;
	virtual void playHitSound() override;
private:
	bool _flip = false;
};

/*class PlayerBulletCreator : public BaseCreator {
public:
	GameObject *createGameObject() const override {
		return new PlayerBullet();
	}
};*/

class BossBullet : public Bullet {
public:
	BossBullet() {
		load(LoaderParams(0, 0, 32, 32, "BOSSBULLET"));
	}
	void setAccelerateUpdator(std::function<Vector2(Vector2, int, int, int)> f) {
		accelerate_updator = f;
	}
	SDL_Rect getBox() {
		return { x + 4, y + 4, 24, 24 };
	}
protected:
	std::function<Vector2(Vector2, int, int, int)> accelerate_updator;
};

class BossFireBullet : public BossBullet {
public:
	BossFireBullet() {
		currentRow = 0;
		currentFrame = 0;
		damage = 25;
	}
	virtual void update() override;
	virtual void playHitSound() override;
};

class BossIceBullet : public BossBullet {
public:
	BossIceBullet() {
		currentRow = 1;
		currentFrame = 0;
		damage = 5;
	}
	virtual void update() override;
	virtual void playHitSound() override;
};

template<typename T>
class BulletCreator : public BaseCreator {
public:
	GameObject *createGameObject() const override {
		return new T();
	}
};