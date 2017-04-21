#include "InputHandler.h"
#include "Game.h"
#include <iostream>

int InputHandler::JoystickDeadZone = 4096;

InputHandler::~InputHandler() {
	clean();
}

int InputHandler::xValue(int joy, int stick) {
	if (joystickValues.size() > (size_t)joy) {
		if (stick == LEFT_JOYSTICK) {
			return int(joystickValues[joy].first.getX());
		} else if (stick == RIGHT_JOYSTICK) {
			return int(joystickValues[joy].second.getX());
		}
	}
	return 0;
}

int InputHandler::yValue(int joy, int stick) {
	if (joystickValues.size() > (size_t)joy) {
		if (stick == 1) {
			return int(joystickValues[joy].first.getY());
		} else if (stick == 2) {
			return int(joystickValues[joy].second.getY());
		}
	}
	return 0;
}

int InputHandler::zValue(int joy) {
	if (joystickZValues.size() > (size_t)joy) {
		return int(joystickZValues[joy]);
	}
	return 0;
}

bool InputHandler::buttonState(int joy, int button) {
	if (joystickButtonsState.size() > (size_t)joy) {
		return joystickButtonsState[joy][button];
	}
	return false;
}

void InputHandler::update() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
		case SDL_QUIT:
			Game::Instance().quit();
			break;
		case SDL_JOYAXISMOTION:
		{
			int joystick = e.jaxis.which;
			if (e.jaxis.axis == AXIS_LEFT_X) {
				if (e.jaxis.value > JoystickDeadZone) {
					joystickValues[joystick].first.setX(1);
				} else if (e.jaxis.value < -JoystickDeadZone) {
					joystickValues[joystick].first.setX(-1);
				} else {
					joystickValues[joystick].first.setX(0);
				}
			}
			if (e.jaxis.axis == AXIS_LEFT_Y) {
				if (e.jaxis.value > JoystickDeadZone) {
					joystickValues[joystick].first.setY(1);
				} else if (e.jaxis.value < -JoystickDeadZone) {
					joystickValues[joystick].first.setY(-1);
				} else {
					joystickValues[joystick].first.setY(0);
				}
			}
			if (e.jaxis.axis == AXIS_RIGHT_X) {
				if (e.jaxis.value > JoystickDeadZone) {
					joystickValues[joystick].second.setX(1);
				} else if (e.jaxis.value < -JoystickDeadZone) {
					joystickValues[joystick].second.setX(-1);
				} else {
					joystickValues[joystick].second.setX(0);
				}
			}
			if (e.jaxis.axis == AXIS_RIGHT_Y) {
				if (e.jaxis.value > JoystickDeadZone) {
					joystickValues[joystick].second.setY(1);
				} else if (e.jaxis.value < -JoystickDeadZone) {
					joystickValues[joystick].second.setY(-1);
				} else {
					joystickValues[joystick].second.setY(0);
				}
			}
			if (e.jaxis.axis == AXIS_LEFT_Z) {
				if (e.jaxis.value > 25600) {
					joystickZValues[joystick] = 1.0;
				} else {
					joystickZValues[joystick] = 0.0;
				}
			}
			break;
		}
		case SDL_JOYBUTTONDOWN:
		{
			int joystick = e.jbutton.which;
			joystickButtonsState[joystick][e.jbutton.button] = true;
			break;
		}
		case SDL_JOYBUTTONUP:
		{
			int joystick = e.jbutton.which;
			joystickButtonsState[joystick][e.jbutton.button] = false;
			break;
		}
		}
	}
	//键盘模拟手柄
	if (SDL_NumJoysticks() > 0)
		return;
	auto keystates = SDL_GetKeyboardState(nullptr);
	if (keystates[SDL_SCANCODE_LEFT] && !keystates[SDL_SCANCODE_RIGHT])
		joystickValues[0].first.setX(-1);
	else if (!keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_RIGHT])
		joystickValues[0].first.setX(1);
	else
		joystickValues[0].first.setX(0);
	if (keystates[SDL_SCANCODE_UP] && !keystates[SDL_SCANCODE_DOWN])
		joystickValues[0].first.setY(-1);
	else if (!keystates[SDL_SCANCODE_UP] && keystates[SDL_SCANCODE_DOWN])
		joystickValues[0].first.setY(1);
	else
		joystickValues[0].first.setY(0);
	if (keystates[SDL_SCANCODE_Z])
		joystickButtonsState[0][XBoxInputNodes::BUTTON_A] = true;
	else
		joystickButtonsState[0][XBoxInputNodes::BUTTON_A] = false;
	if (keystates[SDL_SCANCODE_LSHIFT])
		joystickZValues[0] = 1.0;
	else
		joystickZValues[0] = 0.0;
	if (keystates[SDL_SCANCODE_X])
		joystickButtonsState[0][XBoxInputNodes::BUTTON_B] = true;
	else
		joystickButtonsState[0][XBoxInputNodes::BUTTON_B] = false;
	if (keystates[SDL_SCANCODE_RETURN])
		joystickButtonsState[0][XBoxInputNodes::BUTTON_START] = true;
	else
		joystickButtonsState[0][XBoxInputNodes::BUTTON_START] = false;
}

void InputHandler::clean() {
	for (auto joystick : joysticks) {
		SDL_JoystickClose(joystick);
	}
	joysticks.clear();
	joystickValues.clear();
}

void InputHandler::initJoysticks() {
	//检查 Joystick 子系统是否启动
	if (!SDL_WasInit(SDL_INIT_JOYSTICK)) {
		SDL_InitSubSystem(SDL_INIT_JOYSTICK);
	}
	if (SDL_NumJoysticks() > 0) {
		for (int i = 0; i < SDL_NumJoysticks(); ++i) {
			SDL_Joystick *joystick = SDL_JoystickOpen(i);
			if (SDL_JoystickGetAttached(joystick) == SDL_TRUE) {
				joysticks.push_back(joystick);
				joystickValues.push_back(std::make_pair(Vector2(), Vector2()));
				joystickZValues.push_back(0.0);
				std::vector<bool> tmp;
				for (int j = 0; j < SDL_JoystickNumButtons(joystick); ++j) {
					tmp.push_back(false);
				}
				joystickButtonsState.push_back(tmp);
			} else {
				std::cerr << SDL_GetError() << std::endl;
			}
		}
	} else {	// 转录键盘
		SDL_Joystick *joystick = nullptr;
		joysticks.push_back(joystick);
		joystickValues.push_back(std::make_pair(Vector2(), Vector2()));
		joystickZValues.push_back(0.0);
		std::vector<bool> tmp;
		for (int j = 0; j < 10; ++j) {
			tmp.push_back(false);
		}
		joystickButtonsState.push_back(tmp);
	}
	if (joysticks.size() > 0) {
		SDL_JoystickEventState(SDL_ENABLE);
		joystickInitStatus = true;
		std::cout << joysticks.size() << " keyboard-simulated joystick initialized" << std::endl;
	} else {
		joystickInitStatus = false;
	}
}
