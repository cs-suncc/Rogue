#pragma once
#include <string.h>
#include <map>
#include "GameObject.h"

class BaseCreator {
public:
	virtual GameObject *createGameObject() const = 0;
	virtual ~BaseCreator() {}
};

class GameObjectFactory {
public:
	bool registerType(std::string typeID, BaseCreator *creator);
	GameObject *create(std::string typeID);
private:
	std::map<std::string, BaseCreator *> _creators;
};