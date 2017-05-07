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
	Map(std::string map);
	int **getLayer() {
		return layer;
	}
	Tile *getTile(int line, int column);
	void getViewPort(int *left, int *right) {
		*left = view_left;
		*right = view_right;
	}
	void viewport(int left, int right) {
		view_left = left;
		view_right = right;
	}
	void draw();
private:
	std::string map;
	int **layer;
	tinyxml2::XMLDocument *root;
	Tileset *_tile;
	int view_left;
	int view_right;
	int width, height;
};
