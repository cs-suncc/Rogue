#pragma once
#include "GameObject.h"

class SDLGameObject : public GameObject {
public:
	SDLGameObject();
	virtual void load(const LoaderParams &param) override;
	virtual void draw();
	virtual void update();
	virtual void clean();
	virtual void setCurrentRow(int v);
	virtual void setCurrentFrame(int v);
	int getX() {
		return x;
	}
	int getY() {
		return y;
	}
	int getW() {
		return width;
	}
	int getH() {
		return height;
	}
protected:
	int x;
	int y;
	int width;
	int height;
	std::string textureID;
	int currentRow;
	int currentFrame;
};