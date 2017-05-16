#include "UI.h"
#ifdef _DEBUG
#include <iostream>
#endif

void UI::newUI(std::string id, std::string type, int x, int y, int w, int h, int value_max)
{
	if (type == "Bar") {
		if (uis.find(id) == uis.end()) {
			uis[id] = new UIBar(x, y, w, h, value_max);
		} else {
#ifdef _DEBUG
			std::cerr << "UI Entity " << id << " 已经创建";
#endif
		}
	}
}

bool UI::hasUI(std::string id)
{
	if (uis.find(id) == uis.end())
		return false;
	else
		return true;
}

void UI::destroyUI(std::string id)
{
	if (uis.find(id) == uis.end()) {
#ifdef _DEBUG
		std::cerr << "UI Entity " << id << " 不存在";
#endif
		throw "UI Entity " + id + " 不存在";
	}
	uis.erase(id);
}

void UI::setUIValue(std::string id, int value)
{
	if (uis.find(id) == uis.end()) {
#ifdef _DEBUG
		std::cerr << "UI Entity " << id << " 不存在";
#endif
		throw "UI Entity " + id + " 不存在";
	}
	uis[id]->value = value;
}

void UI::setUIValueMax(std::string id, int value_max)
{
	if (uis.find(id) == uis.end()) {
#ifdef _DEBUG
		std::cerr << "UI Entity " << id << " 不存在";
#endif
		throw "UI Entity " + id + " 不存在";
	}
	uis[id]->value_max = value_max;
}

int UI::getUIValue(std::string id)
{
	if (uis.find(id) == uis.end()) {
#ifdef _DEBUG
		std::cerr << "UI Entity " << id << " 不存在";
#endif
		throw "UI Entity " + id + " 不存在";
	}
	return uis[id]->value;
}

void UI::setUIBarColor(std::string id, Uint8 r, Uint8 g, Uint8 b)
{
	if (uis.find(id) == uis.end()) {
#ifdef _DEBUG
		std::cerr << "UI (Bar)Entity " << id << " 不存在";
#endif
		throw "UI (Bar)Entity " + id + " 不存在";
	}
	auto bar = static_cast<UIBar *>(uis[id]);
	bar->r = r;
	bar->g = g;
	bar->b = b;
}

void UI::draw()
{
	for (auto each : uis) {
		each.second->draw();
	}
}
