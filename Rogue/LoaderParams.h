#pragma once
#include <string>
class LoaderParams {
	/// 用于GameObject族的初始化
public:
	LoaderParams(int _x, int _y, int _w, int _h, std::string _t)
		:x(_x), y(_y), width(_w), height(_h), textureID(_t) {
	}

	int getX() const { return x; }
	int getY() const { return y; }
	int getWidth() const { return width; }
	int getHeight() const { return height; }
	const std::string &getTextureID() const { return textureID; }
private:
	int x;
	int y;
	int width;
	int height;
	std::string textureID;
};
