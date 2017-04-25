#include "MapLoader.h"

Map::Map(std::string map): map(map) {
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
		bool pass = (pv == "true");
		_tile->addTile(atoi(p->Attribute("id")), pass);
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
