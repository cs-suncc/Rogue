#pragma once
#include "TextureManager.h"
#include "SDL_image.h"
#include <iostream>
#include <fstream>

TextureManager::~TextureManager() {
	for (auto each : textureMap) {
		SDL_DestroyTexture(each.second);
	}
	textureMap.clear();
}


bool TextureManager::load(std::string fileName, std::string id, SDL_Renderer* renderer) {
	//读取fileName所指图像，加载为Surface，并创建Texture
	auto tmp_surface = IMG_Load(fileName.c_str());

	if (nullptr == tmp_surface)
	{
		std::cerr << "No such file\n" << std::endl;
		std::ofstream fout("error.log");
		fout << SDL_GetError() << std::endl;
		return false;
	}
	auto tmp_texture = SDL_CreateTextureFromSurface(renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);
	if (nullptr == tmp_texture)
		return false;
	SDL_SetTextureBlendMode(tmp_texture, SDL_BLENDMODE_BLEND);
	//Texture -> textureMap
	textureMap[id] = tmp_texture;
	return true;
}


void TextureManager::draw(std::string id, int x, int y, int width, int height, SDL_Renderer *renderer, SDL_RendererFlip flip) {
	SDL_Rect srcRect;
	SDL_Rect dstRect;

	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = width;
	srcRect.h = height;

	dstRect.x = x;
	dstRect.y = y;
	dstRect.w = width;
	dstRect.h = height;

	SDL_RenderCopyEx(renderer, textureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}


void TextureManager::drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer * renderer, SDL_RendererFlip flip) {
	SDL_Rect srcRect;
	SDL_Rect dstRect;

	srcRect.x = width * currentFrame;
	srcRect.y = height * currentRow;  //根据row和frame计算源矩形的坐标
	srcRect.w = width;
	srcRect.h = height;

	dstRect.x = x;
	dstRect.y = y;
	dstRect.w = width;
	dstRect.h = height;

	SDL_RenderCopyEx(renderer, textureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::drawFrameRotate(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, double angle, SDL_Point * center, SDL_Renderer * renderer, SDL_RendererFlip flip) {
	SDL_Rect srcRect;
	SDL_Rect dstRect;

	srcRect.x = width * currentFrame;
	srcRect.y = height * currentRow;  //根据row和frame计算源矩形的坐标
	srcRect.w = width;
	srcRect.h = height;

	dstRect.x = x;
	dstRect.y = y;
	dstRect.w = width;
	dstRect.h = height;

	SDL_RenderCopyEx(renderer, textureMap[id], &srcRect, &dstRect, angle, center, flip);
}
