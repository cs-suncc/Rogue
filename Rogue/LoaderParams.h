#pragma once
#include <string>
class LoaderParams {
	/// 用于GameObject族的初始化
public:
	LoaderParams(int x, int y, int w, int h, std::string textureID)
		:x(x), y(y), width(w), height(h), textureID(textureID) {
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
