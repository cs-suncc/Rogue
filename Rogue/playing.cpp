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
	for (auto gem : healgems) {
		gem->update();
	}
	for (auto gem : managems) {
		gem->update();
	}
	for (auto blt = bullets.begin(); blt != bullets.end();) {
		(*blt)->update();
		if ((*blt)->perish()) {
			blt = bullets.erase(blt);
		} else {
			++blt;
		}
	}
	for (auto blt = boss_bullets.begin(); blt != boss_bullets.end();) {
		(*blt)->update();
		if ((*blt)->perish()) {
			blt = boss_bullets.erase(blt);
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
		for (auto gem : healgems) {
			gem->setX(gem->getX() - 480);
		}
		for (auto gem : managems) {
			gem->setX(gem->getX() - 480);
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
			for (auto gem : healgems) {
				gem->setX(gem->getX() + 480);
			}
			for (auto gem : managems) {
				gem->setX(gem->getX() + 480);
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
			for (auto gem : healgems) {
				gem->setX(gem->getX() + t);
			}
			for (auto gem : managems) {
				gem->setX(gem->getX() + t);
			}
		}
		MapManager::Instance().getMap(currentMap)->viewport(left, right);
	}
	//中间存档
	if (left >= 5280 && !Game::Instance().middleSaved()) {
		Game::Instance().middleSave();
	}
	//处理伤害
	SDL_Rect player_box = { player->getX() + 5, player->getY() - 20, 20, 50 };
	if (!player->immutable()) {
		//处理伤害 之 体术
		for (auto enmp : enemys) {
			if (collision(player_box, enmp->getBox())) {
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
		//处理伤害 之 魔法
		for (auto bltp = boss_bullets.begin(); bltp != boss_bullets.end(); ) {
			auto blt = static_cast<BossBullet *>(*bltp);
			if (collision(player_box, blt->getBox())) {
				if (!player->isDefending()) {
					player->setHitpoint(player->getHitpoint() - blt->getDamage());
					UI::Instance().setUIValue("PlayerHP", player->getHitpoint());
					blt->playHitSound();
				}
				bltp = boss_bullets.erase(bltp);
			} else {
				++bltp;
			}
		}
	} else {
#ifdef _DEBUG
		std::cerr << "Player Immutable" << std::endl;
#endif // _DEBUG
	}
	//处理道具
	if (player->getHitpoint() != player->getHitpointMax()) {
		for (auto each : healgems) {
			if (collision({ player->getX() + 5, player->getY() - 20, 20, 50 }, each->getBox())) {
				auto hp = player->getHitpoint();
				hp = hp + 40 <= player->getHitpointMax() ? hp + 40 : player->getHitpointMax();
				player->setHitpoint(hp);
				UI::Instance().setUIValue("PlayerHP", player->getHitpoint());
				AudioManager::Instance().playSound("HEAL");
				each->activate();
				break;
			}
		}
	}
	if (player->getMana() != player->getManaMax()) {
		for (auto each : managems) {
			if (collision({ player->getX() + 5, player->getY() - 20, 20, 50 }, each->getBox())) {
				auto mp = player->getMana();
				mp = mp + 100 <= player->getManaMax() ? mp + 40 : player->getManaMax();
				player->setMana(mp);
				UI::Instance().setUIValue("PlayerMana", player->getMana());
				AudioManager::Instance().playSound("MANA");
				each->activate();
				break;
			}
		}
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
	for (auto blt : boss_bullets) {
		blt->draw();
	}
	for (auto enm : enemys) {
		enm->draw();
	}
	for (auto gem : healgems) {
		gem->draw();
	}
	for (auto gem : managems) {
		gem->draw();
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
	TextureManager::Instance().load("asset/image/BossBullet.png", "BOSSBULLET", Game::Instance().getRenderer());
	TextureManager::Instance().load("asset/image/EnemyBat.png", "ENEMYBAT", Game::Instance().getRenderer());
	TextureManager::Instance().load("asset/image/EnemyZombie.png", "ENEMYZOMBIE", Game::Instance().getRenderer());
	TextureManager::Instance().load("asset/image/EnemyBoss.png", "BOSS", Game::Instance().getRenderer());
	TextureManager::Instance().load("asset/image/Gem.png", "GEM", Game::Instance().getRenderer());
	AudioManager::Instance().loadMusic("asset/audio/dungeon.mp3", "PLAYINGBGM");
	AudioManager::Instance().loadSound("asset/audio/PlayerAttack.mp3", "PLAYERATTACK");
	AudioManager::Instance().loadSound("asset/audio/PlayerMagic.mp3", "PLAYERMAGIC");
	AudioManager::Instance().loadSound("asset/audio/PlayerDefending.mp3", "PLAYERDEFENDING");
	AudioManager::Instance().loadSound("asset/audio/PlayerJump.mp3", "PLAYERJUMPING");
	AudioManager::Instance().loadSound("asset/audio/AttackHit.mp3", "ATTACKHIT");
	AudioManager::Instance().loadSound("asset/audio/MagicHit.mp3", "MAGICHIT");
	AudioManager::Instance().loadSound("asset/audio/EnemyDestroy.mp3", "ENEMYDESTROY");
	AudioManager::Instance().loadSound("asset/audio/Heal.mp3", "HEAL");
	AudioManager::Instance().loadSound("asset/audio/Mana.mp3", "MANA");
	AudioManager::Instance().loadSound("asset/audio/BossSkill.mp3", "BOSSSKILL");
	AudioManager::Instance().loadSound("asset/audio/BossFireHit.mp3", "BOSSFIREHIT");
	AudioManager::Instance().loadSound("asset/audio/BossIceHit.mp3", "BOSSICEHIT");

	auto &factories = Game::Instance().factories();

	SDL_SetRenderDrawColor(Game::Instance().getRenderer(), 227, 227, 227, 255);
	if (!Game::Instance().middleSaved()) {
		auto *player = (Player *)factories.create("Player");
		player->load(LoaderParams(0 * 32, 13 * 32, 32, 64, "PLAYER"));
		player->setCurrentRow(0);
		player->setCurrentFrame(0);
		this->player = player;
	} else {
		auto *player = (Player *)factories.create("Player");
		player->load(LoaderParams(4 * 32, 12 * 32, 32, 64, "PLAYER"));
		player->setCurrentRow(0);
		player->setCurrentFrame(0);
		this->player = player;
	}
	auto allbats = MapManager::Instance().getMap(currentMap)->getBatSpawners();
	for (auto each : allbats) {
		auto bat = static_cast<EnemyBat *>(factories.create("EnemyBat"));
		bat->setMaxHitpoint(40);
		bat->setHitpoint(40);
		bat->setX(each.first * 32);
		bat->setY(each.second * 32 + 10);
		enemys.push_back(bat);
	}

	auto allzombies = MapManager::Instance().getMap(currentMap)->getZombieSpawners();
	for (auto each : allzombies) {
		auto zombie = static_cast<EnemyZombie *>(factories.create("EnemyZombie"));
		zombie->setMaxHitpoint(200);
		zombie->setHitpoint(200);
		zombie->setX(each.first * 32);
		zombie->setY(each.second * 32 - 17);
		enemys.push_back(zombie);
	}

	auto allhealgems = MapManager::Instance().getMap(currentMap)->getHealerSpawners();
	for (auto each : allhealgems) {
		auto gem = static_cast<HealGem *>(factories.create("HealGem"));
		gem->setX(each.first * 32);
		gem->setY(each.second * 32);
		healgems.push_back(gem);
	}

	auto allmanagems = MapManager::Instance().getMap(currentMap)->getManaSpawners();
	for (auto each : allmanagems) {
		auto gem = static_cast<ManaGem *>(factories.create("ManaGem"));
		gem->setX(each.first * 32);
		gem->setY(each.second * 32);
		managems.push_back(gem);
	}
	
	/// TEST
	auto bblt = static_cast<BossFireBullet *>(factories.create("BossFireBullet"));
	bblt->setX(20 * 32);
	bblt->setY(10 * 32);
	bblt->setVelocity(Vector2(-2, 0));
	bblt->setAccelerateUpdator(
	[](Vector2 a, int _, int __, int ___) {
		return a;
	});
	addBullet(bblt);
	toBossBattle();

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

void PlayingState::addBullet(BossBullet * blt) {
	boss_bullets.push_back(blt);
}

void PlayingState::toBossBattle()
{
	MapManager::Instance().loadMap("BOSS", "asset/Boss.tmx");
	currentMap = "BOSS";
	enemys.clear();
	bullets.clear();
	healgems.clear();
	managems.clear();
	auto &factories = Game::Instance().factories();
	auto allhealgems = MapManager::Instance().getMap(currentMap)->getHealerSpawners();
	for (auto each : allhealgems) {
		auto gem = static_cast<HealGem *>(factories.create("HealGem"));
		gem->setX(each.first * 32);
		gem->setY(each.second * 32);
		healgems.push_back(gem);
	}

	auto allmanagems = MapManager::Instance().getMap(currentMap)->getManaSpawners();
	for (auto each : allmanagems) {
		auto gem = static_cast<ManaGem *>(factories.create("ManaGem"));
		gem->setX(each.first * 32);
		gem->setY(each.second * 32);
		managems.push_back(gem);
	}
	enemys.push_back(static_cast<Boss *>(factories.create("Boss")));
	player->setX(0);
	player->setY(17 * 32);
}
