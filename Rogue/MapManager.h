#pragma once
#include "MapLoader.h"
#include <map>
#include <string>

class MapManager {
public:
	inline static MapManager &Instance() {
		static MapManager singleton;
		return singleton;
	}
	Map *getMap(std::string ID);
	void loadMap(std::string ID, std::string path);
private:
	std::map<std::string, Map *> maps;

	explicit MapManager() {};
	MapManager(MapManager &ref) {};
	~MapManager() {};
	MapManager & operator=(const MapManager &ref) {};
};