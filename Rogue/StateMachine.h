#pragma once
#include "GameState.h"
#include <stack>

class StateMachine {
	//下推自动机
public:
	void pushState(GameState *state);
	void changeState(GameState *state);
	void popState();
	void popState(int tilldown);
	void update();
	void render();
	GameState *currentState();
private:
	std::stack<GameState *> _stack;
};