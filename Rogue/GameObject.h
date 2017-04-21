#pragma once
#include "LoaderParams.h"
class GameObject
{
public:
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void clean() = 0;
	virtual void load(const LoaderParams &param) = 0;
protected:
	GameObject() = default;
	virtual ~GameObject() = default;
};
