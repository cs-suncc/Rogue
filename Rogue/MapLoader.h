#pragma once
#include "tinyxml2.h"
#include <string>
#include <map>
#include "TextureManager.h"
#include "Game.h"
#include <cstdlib>
#include <iostream>
#include <cstring>
class Tile {
public:
	Tile(int id, bool pass) : id(id), pass(pass) {}
	bool passable() { return pass; }
	void setPass(bool p) { pass = p; }
private:
	int id;
	bool pass;
};

class Tileset {
public:
	Tileset(int width, int height, int columns) :
		width(width), height(height), columns(columns) {
	}
	void loadTileset(std::string textureID, std::string path) {
		TextureManager::Instance().load(std::string("asset/") + path, textureID, Game::Instance().getRenderer());
		_tile_textureID = textureID;
	}
	void addTile(int id, bool pass) {
		_tiles[id] = new Tile(id, pass);
	}
	Tile *getTile(int id) {
		return _tiles[id];
	}
	void draw(int id, int x, int y) {
		TextureManager::Instance().drawFrame(
			_tile_textureID,
			x, y,
			width, height,
			id / columns, (id - 1) % columns,
			Game::Instance().getRenderer()
		);
	}
private:
	int width, height;
	int columns;
	std::string _tile_textureID;
	std::map<int, Tile *> _tiles;
};

class Map {
public:
	Map(std::string map) :map(map) {
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
			std::cerr << p->Attribute("id") << std::endl;
			std::string pv = p->FirstChildElement("properties")->FirstChildElement("property")->Attribute("value");
			bool pass = (pv == "true");
			_tile->addTile(atoi(p->Attribute("id")), pass);
			p = p->NextSiblingElement();
		}
		p = root->FirstChildElement("map")->FirstChildElement("layer")->FirstChildElement("data");
		char *td = (char *)malloc((strlen(p->FirstChild()->ToText()->Value()) + 1) * sizeof(char));
		std::cerr << strlen(p->FirstChild()->ToText()->Value()) + 1 << std::endl;
		strcpy_s(td, (strlen(p->FirstChild()->ToText()->Value()) + 1) * sizeof(char), p->FirstChild()->ToText()->Value());
		int offset = 0;
		auto ttd = td;
		for (int i = 0; i < height; ++i)
			for (int j = 0; j < width; ++j) {
				td += offset;
				sscanf_s(td, "%d,%n", &layer[i][j], &offset);
			}
		delete ttd;
		for (int i = 0; i < height; ++i, std::cout<<std::endl)
			for (int j = 0; j < width; ++j) {
				std::cout << layer[i][j] << ' ';
			}
	}
	int width, height;
	int **layer;
	Tileset *_tile;
private:
	std::string map;

	tinyxml2::XMLDocument *root;
};
