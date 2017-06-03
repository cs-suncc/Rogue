#pragma once
#include "GameState.h"
#include "SDLGameObject.h"

class MainMenuState : public GameState {
public:
	void update() override;
	void render() override;
	bool onEnter() override;
	bool onExit() override;

	std::string getStateID() const override {
		return menuID;
	}
private:
	static const std::string menuID;
	std::vector<GameObject *> gameObjects;
	std::vector<SDLGameObject *> mainmenubuttons;
	GameObject *title;
	GameObject *help;
	int selected = 0;
};
