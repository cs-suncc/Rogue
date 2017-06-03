#pragma once
#include "GameState.h"
#include <stack>

class StateMachine {
	//�����Զ���
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