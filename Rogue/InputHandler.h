#pragma once
#include "SDL.h"
#include "Vector2.h"
#include <vector>

enum XBoxInputNodes {
	LEFT_JOYSTICK = 1,
	RIGHT_JOYSTICK = 2,
	AXIS_LEFT_X = 0,
	AXIS_LEFT_Y = 1,	//		LZ					    RZ
	AXIS_LEFT_Z = 2,	//		   LB				RB
	AXIS_RIGHT_X = 3,	//		HAT		BACK	START	Y
	AXIS_RIGHT_Y = 4,	//	HAT	 +	HAT				X		B
	AXIS_RIGHT_Z = 5,	//		HAT						A
	BUTTON_A = 0,		//			LSTICK		RSTICK
	BUTTON_B = 1,
	BUTTON_X = 2,
	BUTTON_Y = 3,
	BUTTON_LB = 4,
	BUTTON_RB = 5,
	BUTTON_BACK = 6,
	BUTTON_START = 7,
	BUTTON_LSTICK = 8,
	BUTTON_RSTICK = 9,
	HAT_NONE = 0,
	HAT_UP = 1,
	HAT_RIGHT = 2,
	HAT_DOWN = 4,
	HAT_LEFT = 8,
};

class InputHandler {
private:
	InputHandler() {}
	InputHandler(const InputHandler &ref) {}
	const InputHandler& operator = (const InputHandler &ref) {} // 封闭构造与拷贝，构造Singleton
	~InputHandler(); //保证 Heap-Based

	std::vector<SDL_Joystick *> joysticks;
	std::vector<std::pair<Vector2, Vector2>> joystickValues;
	std::vector<double> joystickZValues;
	std::vector<std::vector<bool>> joystickButtonsState;
	bool joystickInitStatus;

public:

	static int JoystickDeadZone;

	inline static InputHandler &Instance() {
		static InputHandler singleton;
		return singleton;
	}

	void update();
	void clean();

	void initJoysticks();
	bool isJoysticksInit() const { return joystickInitStatus; }

	int xValue(int joy, int stick);
	int yValue(int joy, int stick);
	int zValue(int joy);

	bool buttonState(int joy, int button);
};
