#include "MapManager.h"

Map *MapManager::getMap(std::string ID)
{
	return maps[ID];
}

void MapManager::loadMap(std::string ID, std::string path)
{
	if (maps.find(ID) == maps.end()) {
		auto p = new Map(path);
		maps[ID] = p;
	} else {
#ifdef _DEBUG
		std::cerr << "Map ID " << ID << " has been loaded" << std::endl;
#endif
	}
}
