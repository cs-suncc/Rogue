#pragma once
#include "GameState.h"
#include "SDLGameObject.h"
class GameOverState : public GameState {
public:
	void update() override;
	void render() override;
	bool onEnter() override;
	bool onExit() override;

	std::string getStateID() const override {
		return "GameOver";
	}
private:
	std::vector<GameObject *> gameObjects;

	int selected = 0;
};
