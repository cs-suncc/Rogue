#include "StateMachine.h"

void StateMachine::pushState(GameState *state)
{
	_stack.push(state);
	_stack.top()->onEnter();
}

void StateMachine::changeState(GameState *state)
{
	if (!_stack.empty()) {
		if (_stack.top()->getStateID() == state->getStateID()) {
			return;
		}
		if (_stack.top()->onExit()) {
			delete _stack.top();
			_stack.pop();
		}
	}
	pushState(state);
}

void StateMachine::popState()
{
	if (!_stack.empty()) {
		if (_stack.top()->onExit()) {
			delete _stack.top();
			_stack.pop();
		}
	}
}
