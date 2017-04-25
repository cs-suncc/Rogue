#pragma once
#include "GameState.h"
#include "MapLoader.h"

class PlayingState : public GameState {
public:
	void update() override;
	void render() override;
	bool onEnter() override;
	bool onExit() override;

	std::string getStateID() const override {
		return playingID;
	}
private:
	static const std::string playingID;
	std::vector<GameObject *> gameObjects;
	Map *mp;
	int viewport;
};
