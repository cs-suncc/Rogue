#include "Bullet.h"
#include "AudioManager.h"

void BossFireBullet::update() {
	if (x < -32 || x > 960 || y < -32 || y > 640)
		destroy = true;
	accelerate = accelerate_updator(accelerate, livetime, x, y);
	++livetime;
	velocity += accelerate;
	x += velocity.getX();
	y += velocity.getY();
	currentFrame = (currentFrame + 1) % 4;
}

void BossFireBullet::playHitSound() {
	AudioManager::Instance().playSound("BOSSFIREHIT");
}
