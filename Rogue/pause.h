#pragma once
#include "GameState.h"
#include "SDLGameObject.h"

class PauseState : public GameState {
public:
	void update() override;
	void render() override;
	bool onEnter() override;
	bool onExit() override;

	std::string getStateID() const override {
		return "Pause";
	}
private:
	std::vector<GameObject *> gameObjects;
	std::vector<SDLGameObject *> buttons;
	int selected = 0;
};
