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
	InputHandler::Instance().update();
	if (InputHandler::Instance().buttonState(0, XBoxInputNodes::BUTTON_BACK)) {
		Game::Instance().getGameStateMachine()->popState();
		AudioManager::Instance().playMusic("TITLEBGM");
		return;
	}
	player->update();
	for (auto blt : bullets) {
		blt->update();
	}
	for (auto enm = enemys.begin(); enm != enemys.end();) {
		(*enm)->update();
		if ((*enm)->perish()) {
			enm = enemys.erase(enm);
		} else {
			++enm;
		}
	}
	//处理伤害
		//todo
	//处理对敌伤害
	for (auto blt = bullets.begin(); blt != bullets.end();) {
		auto bltp = *blt;
		bool kflag = false;
		SDL_Rect bulletBox = { bltp->getX() - bltp->getVelocity().getX(),
							   bltp->getY(),
							   bltp->getVelocity().getX() + bltp->getW(),
						       bltp->getH()};
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
				kflag = true;
				++enm;
			} else {
				++enm;
			}
		}
		if (kflag) {
			blt = bullets.erase(blt);
		} else {
			++blt;
		}
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
	TextureManager::Instance().load("asset/image/rogue2.png", "PLAYER", Game::Instance().getRenderer());
	TextureManager::Instance().load("asset/image/bullet.png", "BULLET", Game::Instance().getRenderer());
	TextureManager::Instance().load("asset/image/EnemyBat.png", "ENEMYBAT", Game::Instance().getRenderer());
	AudioManager::Instance().loadMusic("asset/audio/dungeon.mp3", "PLAYINGBGM");
	AudioManager::Instance().loadSound("asset/audio/PlayerAttack.mp3", "PLAYERATTACK");
	AudioManager::Instance().loadSound("asset/audio/PlayerMagic.mp3", "PLAYERMAGIC");
	AudioManager::Instance().loadSound("asset/audio/PlayerDefending.mp3", "PLAYERDEFENDING");

	SDL_SetRenderDrawColor(Game::Instance().getRenderer(), 227, 227, 227, 255);

	auto *player = (Player *)Game::Instance().factories().create("Player");
	player->load(LoaderParams(0 * 32, 13 * 32, 32, 64, "PLAYER"));
	player->setCurrentRow(0);
	player->setCurrentFrame(0);
	this->player = player;
	auto *bat = static_cast<EnemyBat *>(Game::Instance().factories().create("EnemyBat"));
	bat->setMaxHitpoint(40);
	bat->setHitpoint(40);
	bat->setX(23 * 32);
	bat->setY(8 * 32);
	enemys.push_back(bat);
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
