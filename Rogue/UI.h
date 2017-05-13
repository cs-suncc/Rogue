#pragma once
#include "Game.h"
#include <string>
#include <map>

struct UIEntity {
	int x;
	int y;
	int w;
	int h;
	int value;
	int value_max;
	virtual void draw() = 0;
	UIEntity(int x, int y, int w, int h, int value_max)
		:x(x), y(y), w(w), h(h), value(value_max), value_max(value_max) {
	}
};

class UI {
public:
	static UI &Instance() {
		static UI singleton;
		return singleton;
	}
	void newUI(std::string id, std::string type, int x, int y, int w, int h, int value_max);
	void destroyUI(std::string id);
	void setUIValue(std::string id, int value);
	void setUIValueMax(std::string id, int value_max);
	int getUIValue(std::string id);
	void setUIBarColor(std::string id, Uint8 r, Uint8 g, Uint8 b);
	void draw();
private:
	std::map<std::string, UIEntity *> uis;
	UI() {}
	UI(const UI & ref) {}
	~UI() {}
};

struct UIBar : public UIEntity {
	UIBar(int x, int y, int w, int h, int value_max);
	Uint8 r;
	Uint8 g;
	Uint8 b;
	virtual void draw() override;
};