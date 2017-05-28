#include <iostream>
#include "playing.h"
#include "TextureManager.h"
#include "InputHandler.h"
#include "Game.h"
#include "SDLGameObject.h"
#include "Player.h"
#include "GameObjectFactory.h"
#include "MapManager.h"
#include "AudioManager.h"
#include "Enemy.h"
#include "UI.h"

const std::string PlayingState::playingID = "PLAYING";

void PlayingState::update() {
	static int dying_noupdate = 0;
	static bool on_counting = false;
	InputHandler::Instance().update();
	if (InputHandler::Instance().buttonState(0, XBoxInputNodes::BUTTON_BACK)) {
		Game::Instance().getGameStateMachine()->popState();
		AudioManager::Instance().playMusic("TITLEBGM");
		return;
	}
	if (player->getHitpoint() <= 0) {
		Game::Instance().getGameStateMachine()->popState();
		AudioManager::Instance().playMusic("TITLEBGM");
		return;
	}
	try {
		player->update();
	}
	catch(std::exception e) {
		Game::Instance().getGameStateMachine()->popState();
		AudioManager::Instance().playMusic("TITLEBGM");
		return;
	}
	//视图卷动
	if (player->getX() > 900 && right < MapManager::Instance().getMap(currentMap)->getWidth()*32) {
		left += 480;
		right += 480;
		player->setX(player->getX() - 480);
		for (auto blt : bullets) {
			blt->setX(blt->getX() - 480);
		}
		for (auto enm : enemys) {
			enm->setX(enm->getX() - 480);
		}
		MapManager::Instance().getMap(currentMap)->viewport(left, right);
	} else 	if (player->getX() < 100 && left > 0) {
		auto t = left;
		if (left - 480 > 0) {
			left -= 480;
			right -= 480;
			player->setX(player->getX() + 480);
			for (auto blt : bullets) {
				blt->setX(blt->getX() + 480);
			}
			for (auto enm : enemys) {
				enm->setX(enm->getX() + 480);
			}
		} else {
			left -= t;
			right -= t;
			player->setX(player->getX() + t);
			for (auto blt : bullets) {
				blt->setX(blt->getX() + t);
			}
			for (auto enm : enemys) {
				enm->setX(enm->getX() + t);
			}
		}
		MapManager::Instance().getMap(currentMap)->viewport(left, right);
	}
	//中间存档
	if (left >= 5280 && !Game::Instance().middleSaved()) {
		Game::Instance().middleSave();
	}
	for (auto blt = bullets.begin(); blt != bullets.end();) {
		(*blt)->update();
		if ((*blt)->perish()) {
			blt = bullets.erase(blt);
		} else {
			++blt;
		}
	}
	for (auto enm = enemys.begin(); enm != enemys.end();) {
		(*enm)->update();
		if ((*enm)->perish()) {
			enm = enemys.erase(enm);
			on_counting = true;
		} else {
			++enm;
		}
	}
	if (on_counting && dying_noupdate > 60) {
		UI::Instance().destroyUI("EnemyBar");
		dying_noupdate = 0;
		on_counting = false;
	}
	//处理伤害
	if (!player->immutable()) {
		for (auto enmp : enemys) {
			if (collision({ player->getX() + 5, player->getY() - 20, 20, 50 }, enmp->getBox())) {
				auto damage = 0;
				if (enmp->getType() == "EnemyZombie")
					damage = 25;
				else
					damage = 20;
				if (player->isDefending()) {
					player->setHitpoint(player->getHitpoint() - damage + 15);
					UI::Instance().setUIValue("PlayerHP", player->getHitpoint());
					player->setImmutable();
				} else {
					player->setHitpoint(player->getHitpoint() - damage);
					UI::Instance().setUIValue("PlayerHP", player->getHitpoint());
					player->setImmutable();
				}
			}
		}
	} else {
#ifdef _DEBUG
		std::cerr << "Player Immutable" << std::endl;
#endif // _DEBUG
	}
	//处理对敌伤害
	bool everhit = false;
	for (auto blt = bullets.begin(); blt != bullets.end();) {
		auto bltp = *blt;
		bool kflag = false;
		SDL_Rect bulletBox = { int(bltp->getX() - bltp->getVelocity().getX() + 5),
							   bltp->getY() + 5,
							   int(bltp->getVelocity().getX() + bltp->getW() - 10),
						       bltp->getH() - 10};
		for (auto enm = enemys.begin(); enm != enemys.end();) {
			auto enmp = *enm;
			auto enemyBox = enmp->getBox();
			
			if (collision(bulletBox, enemyBox)) {
				enmp->setHitpoint(enmp->getHitpoint() - bltp->getDamage());
				if (!UI::Instance().hasUI("EnemyBar")) {
					UI::Instance().newUI("EnemyBar","Bar", 560, 0, 400, 15, 0);
					UI::Instance().setUIBarColor("EnemyBar", 200, 50, 50);
				}
				UI::Instance().setUIValueMax("EnemyBar", enmp->getMaxHitpoint());
				UI::Instance().setUIValue("EnemyBar", enmp->getHitpoint());
				dying_noupdate = 0;
				on_counting = false;
				kflag = true;
				++enm;
			} else {
				++enm;
			}
		}
		if (kflag) {
			bltp->playHitSound();
			blt = bullets.erase(blt);
			everhit = true;
		} else {
			++blt;
		}
	} //END 处理敌对伤害
	if (on_counting && !everhit) {
		++dying_noupdate;
	}
}

void PlayingState::render() {
	SDL_SetRenderDrawColor(Game::Instance().getRenderer(), 227, 227, 227, 255);
	SDL_RenderClear(Game::Instance().getRenderer());
	MapManager::Instance().getMap(currentMap)->draw();
	player->draw();
	for (auto blt : bullets) {
		blt->draw();
	}
	for (auto enm : enemys) {
		enm->draw();
	}
	UI::Instance().draw();
}

bool PlayingState::onEnter() {
	MapManager::Instance().loadMap("TESTMAP", "asset/test.tmx");
	currentMap = "TESTMAP";
	if (!Game::Instance().middleSaved()) {
		left = 0;
		right = 30 * 32;
	} else {
		left = 165 * 32;
		right = left + 30 * 32;
	}
	MapManager::Instance().getMap(currentMap)->viewport(left, right);
	TextureManager::Instance().load("asset/image/rogue2.png", "PLAYER", Game::Instance().getRenderer());
	TextureManager::Instance().load("asset/image/bullet.png", "BULLET", Game::Instance().getRenderer());
	TextureManager::Instance().load("asset/image/EnemyBat.png", "ENEMYBAT", Game::Instance().getRenderer());
	TextureManager::Instance().load("asset/image/EnemyZombie.png", "ENEMYZOMBIE", Game::Instance().getRenderer());
	TextureManager::Instance().load("asset/image/Gem.png", "GEM", Game::Instance().getRenderer());
	AudioManager::Instance().loadMusic("asset/audio/dungeon.mp3", "PLAYINGBGM");
	AudioManager::Instance().loadSound("asset/audio/PlayerAttack.mp3", "PLAYERATTACK");
	AudioManager::Instance().loadSound("asset/audio/PlayerMagic.mp3", "PLAYERMAGIC");
	AudioManager::Instance().loadSound("asset/audio/PlayerDefending.mp3", "PLAYERDEFENDING");
	AudioManager::Instance().loadSound("asset/audio/PlayerJump.mp3", "PLAYERJUMPING");
	AudioManager::Instance().loadSound("asset/audio/AttackHit.mp3", "ATTACKHIT");
	AudioManager::Instance().loadSound("asset/audio/MagicHit.mp3", "MAGICHIT");
	AudioManager::Instance().loadSound("asset/audio/EnemyDestroy.mp3", "ENEMYDESTROY");

	SDL_SetRenderDrawColor(Game::Instance().getRenderer(), 227, 227, 227, 255);
	if (!Game::Instance().middleSaved()) {
		auto *player = (Player *)Game::Instance().factories().create("Player");
		player->load(LoaderParams(0 * 32, 13 * 32, 32, 64, "PLAYER"));
		player->setCurrentRow(0);
		player->setCurrentFrame(0);
		this->player = player;
	} else {
		auto *player = (Player *)Game::Instance().factories().create("Player");
		player->load(LoaderParams(4 * 32, 12 * 32, 32, 64, "PLAYER"));
		player->setCurrentRow(0);
		player->setCurrentFrame(0);
		this->player = player;
	}
	auto allbats = MapManager::Instance().getMap(currentMap)->getBatSpawners();

	for (auto each : allbats) {
		auto *bat = static_cast<EnemyBat *>(Game::Instance().factories().create("EnemyBat"));
		bat->setMaxHitpoint(40);
		bat->setHitpoint(40);
		bat->setX(each.first * 32);
		bat->setY(each.second * 32 + 10);
		enemys.push_back(bat);
	}
	auto allzombies = MapManager::Instance().getMap(currentMap)->getZombieSpawners();
	for (auto each : allzombies) {
		auto *zombie = static_cast<EnemyZombie *>(Game::Instance().factories().create("EnemyZombie"));
		zombie->setMaxHitpoint(200);
		zombie->setHitpoint(200);
		zombie->setX(each.first * 32);
		zombie->setY(each.second * 32 - 17);
		enemys.push_back(zombie);
	}
	AudioManager::Instance().playMusic("PLAYINGBGM");
	return true;
}

bool PlayingState::onExit() {
	player->clean();
	for (auto blt : bullets) {
		blt->clean();
		delete blt;
	}
	for (auto enm : enemys) {
		enm->clean();
		delete enm;
	}
	bullets.clear();
	enemys.clear();
#ifdef _DEBUG
	std::cerr << "Playing state - EXIT" << std::endl;
#endif

	return true;
}

void PlayingState::addBullet(Bullet * blt) {
	bullets.push_back(blt);
}
