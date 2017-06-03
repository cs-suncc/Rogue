#include "Enemy.h"
#include "playing.h"
#include "Game.h"
#include "AudioManager.h"
#include <cmath>

Boss::Boss() {
	load(LoaderParams(440, 156, 80, 96, "BOSS"));
	currentRow = 0;
	currentFrame = 0;
	next_frame = 0;
	hitpoint_max = 4000;
	hitpoint = 4000;
	counter = 0;
}

void Boss::update() {
	if (hitpoint <= 0) {
		dying = true;
		return;
	}
	int mx = x + width / 2 - 16;
	int my = y + height / 2 - 16;
	auto _state = static_cast<PlayingState *>(Game::Instance().getGameStateMachine()->currentState());
	auto factories = Game::Instance().factories();
	switch (currentState) {
	case STALL:
		++counter;
		if (next_frame++ == 5) {
			currentFrame = (currentFrame + 1) % 4;
			next_frame = 0;
		}
		if (counter == 100) {
			AudioManager::Instance().playSound("BOSSCAST");
			if (rand() % 2) {
				currentRow = 1;
				counter = 0;
				currentState = FIRE;
			} else {
				currentRow = 2;
				counter = 0;
				currentState = ICE;
			}
		}
		if (rand() % 128 == 1) {
			counter = 0;
			currentState = SUMMONBAT;
		}
		if (rand() % 300 == 0) {
			counter = 0;
			currentState = SUMMONZOMBIE;
		}
		break;
	case SUMMONBAT:
		if (counter++ == 50) {
			auto bat = static_cast<BossBat *>(factories.create("BossBat"));
			bat->setX(rand() % 2 ? 10 : 920);
			bat->setY((8 + (rand() % 9 - 4)) * 32);
			if (bat->getX() < 480)
				bat->flip();
			bat->setMaxHitpoint(60);
			bat->setHitpoint(60);
			_state->addEnemy(bat);
		}
		if (counter == 1) {
			AudioManager::Instance().playSound("SUMMONBAT");
		}
		if (counter == 60) {
			//TO STALL
			counter = 30;
			currentState = STALL;
		}
		break;
	case SUMMONZOMBIE:
		if (counter++ == 50) {
			auto zombie = static_cast<EnemyZombie *>(factories.create("EnemyZombie"));
			zombie->setX(15 * 32);
			zombie->setY(14 * 32 - 17);
			zombie->setMaxHitpoint(300);
			zombie->setHitpoint(300);
			_state->addEnemy(zombie);
		}
		if (counter == 1) {
			AudioManager::Instance().playSound("SUMMONZOMBIE");
		}
		if (counter == 60) {
			//TO STALL
			counter = 30;
			currentState = STALL;
		}
		break;
	case FIRE:
		if (counter++ < 60) {
			break;
		}
		for (int i = 0; i < 18; ++i) {
			auto blt = static_cast<BossFireBullet *>(factories.create("BossFireBullet"));
			blt->setX(mx);
			blt->setY(my);
			blt->setVelocity(Vector2(cos(i / 9.0*M_PI) * 4, sin(i / 9.0*M_PI) * 4));
			blt->setAccelerate(const_cast<Vector2 &>(blt->getVelocity()).normalized());
			blt->setAccelerateUpdator(
				[](Vector2 a, int t, int _, int __) {
				return a * (t / 15.0);
			});
			_state->addBullet(blt);
		}
		AudioManager::Instance().playSound("BOSSSKILL");
		counter = 0;
		currentRow = 0;
		currentState = STALL;
		break;
	case ICE:
		if (counter++ % 15 != 14) {
			break;
		}
		for (int i = 0; i < 4; ++i) {
			auto blt = static_cast<BossIceBullet *>(factories.create("BossIceBullet"));
			blt->setX(mx);
			blt->setY(my);
			auto factor = (rand() % 40) / 40.0;
			blt->setVelocity(Vector2(cos(factor*M_PI) * 2, sin(factor*M_PI) * 2));
			blt->setAccelerate(const_cast<Vector2 &>(blt->getVelocity()).normalized());
			blt->setAccelerateUpdator(
				[](Vector2 a, int t, int _, int __) {
				return a;
			});
			_state->addBullet(blt);
		}
		AudioManager::Instance().playSound("BOSSSKILL");
		if (counter++ == 210) {
			counter = 0;
			currentRow = 0;
			currentState = STALL;
		}
		break;
	}
	
}
