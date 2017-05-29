#pragma once
#include "GameState.h"
#include "MapLoader.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
#include "Gem.h"

class PlayingState : public GameState {
public:
	void update() override;
	void render() override;
	bool onEnter() override;
	bool onExit() override;
	std::string getCurrentMap() {
		return currentMap;
	}
	std::string getStateID() const override {
		return playingID;
	}
	void addBullet(Bullet *);
	int getViewportLeft() {
		return left;
	}
private:
	static const std::string playingID;
	Player *player;
	std::vector<Bullet *> bullets;
	std::vector<Enemy *> enemys;
	std::vector<HealGem *> healgems;
	std::vector<ManaGem *> managems;
	int left=0;
	int right = 960;
	std::string currentMap;
};
