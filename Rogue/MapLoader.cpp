#include "MapLoader.h"

auto EmptyTile = new Tile(0x7ff, false);

Map::Map(std::string map): map(map) {
	view_left = 0;
	view_right = 32 * 30 + 1;
	root = new tinyxml2::XMLDocument();
	root->LoadFile(map.c_str());
	height = atoi(root->FirstChildElement("map")->Attribute("height"));
	width = atoi(root->FirstChildElement("map")->Attribute("width"));
	layer = new int*[height];
	for (int i = 0; i < height; ++i)
		layer[i] = new int[width];
	auto p = root->FirstChildElement("map")->FirstChildElement("tileset");
	_tile = new Tileset(atoi(p->Attribute("tilewidth")), atoi(p->Attribute("tileheight")), atoi(p->Attribute("columns")));
	_tile->loadTileset(p->Attribute("name"), p->FirstChildElement("image")->Attribute("source"));
	p = p->FirstChildElement("tile");
	while (p != nullptr) {
#ifdef _DEBUG
		std::cerr << p->Attribute("id") << std::endl;
#endif
		std::string pv = p->FirstChildElement("properties")->FirstChildElement("property")->Attribute("value");
		std::string pn = p->FirstChildElement("properties")->FirstChildElement("property")->Attribute("name");
		bool pass;
		if (pn == "Bat") {
			pass = true;
			_tile->addTile(atoi(p->Attribute("id")), pass, 1);
		} else if (pn == "zombie") {
			pass = true;
			_tile->addTile(atoi(p->Attribute("id")), pass, 2);
		} else {
			pass = (pv == "true");
			_tile->addTile(atoi(p->Attribute("id")), pass);
		}
		p = p->NextSiblingElement();
	}
	p = root->FirstChildElement("map")->FirstChildElement("layer")->FirstChildElement("data");
	char *td = (char *)malloc((strlen(p->FirstChild()->ToText()->Value()) + 1) * sizeof(char));
#ifdef _DEBUG
	std::cerr << strlen(p->FirstChild()->ToText()->Value()) + 1 << std::endl;
#endif
	strcpy_s(td, (strlen(p->FirstChild()->ToText()->Value()) + 1) * sizeof(char), p->FirstChild()->ToText()->Value());
	int offset = 0;
	auto ttd = td;
	for (int i = 0; i < height; ++i)
		for (int j = 0; j < width; ++j) {
			td += offset;
			sscanf_s(td, "%d,%n", &layer[i][j], &offset);
		}
	delete ttd;
#ifdef _DEBUG
	for (int i = 0; i < height; ++i, std::cout << std::endl)
		for (int j = 0; j < width; ++j) {
			std::cout << layer[i][j] << ' ';
		}
#endif
}

Tile *Map::getTile(int line, int column)
{
	if (line >= height || line < 0 || column >= width || column < 0)
		return EmptyTile;
	auto id = layer[line][column];
	return  _tile->getTile(id - 1);
}

void Map::draw()
{
	for (auto i = 0; i < height; ++i) {
		int x = 0;
		for (auto j = view_left; j < view_right; j += 32) {
			auto id = layer[i][j / 32];
			_tile->draw(id, x * 32, i * 32);
			++x;
		}
	}

}

std::vector<std::pair<int, int>> Map::getBatSpawners()
{
	std::vector<std::pair<int, int>> spawners;
	for (int i=0;i<height; ++i)
		for (int j = 0; j < width; ++j) {
			if (getTile(i, j)->spawnableBat())
				spawners.push_back({ j, i }); // x goes first
		}
	return spawners;
}

std::vector<std::pair<int, int>> Map::getZombieSpawners()
{
	std::vector<std::pair<int, int>> spawners;
	for (int i = 0; i<height; ++i)
		for (int j = 0; j < width; ++j) {
			if (getTile(i, j)->spawnableZombie())
				spawners.push_back({ j, i }); // x goes first
		}
	return spawners;
}
