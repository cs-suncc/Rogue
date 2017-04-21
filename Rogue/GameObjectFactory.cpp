#include "GameObjectFactory.h"
#include <iostream>

bool GameObjectFactory::registerType(std::string typeID, BaseCreator *creator) {
	if (_creators.find(typeID) != _creators.end()) {
		delete creator;
		return false;
	}
	_creators[typeID] = creator;
	return true;
}

GameObject *GameObjectFactory::create(std::string typeID) {
	auto it = _creators.find(typeID);
	if (it == _creators.end()) {
#ifdef _DEBUG
		std::cerr << '[' << __FUNCTION__ << ']' << "Unregisted type " << typeID << std::endl;
#endif
		return nullptr;
	} else {
		auto creator = _creators[typeID];
		return creator->createGameObject();
	}
}