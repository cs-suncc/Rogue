#pragma once
#include "SDL.h"
#include <string>
#include <map>

class TextureManager
{
private:
	std::map<std::string, SDL_Texture*> textureMap;

	explicit TextureManager() {}  //No Construction
	TextureManager(const TextureManager &ref) {}  //No Copy Construction
	TextureManager & operator = (const TextureManager &ref) {}  //No Copy Assignment£¬TexturManager is a singleton

	~TextureManager();  //private Destructor, force heap-based bbject

public:
	inline static TextureManager& Instance() {
		static TextureManager singleton;  //Traget
		return singleton;
	}  //Singleton

	/// Load a new texture
	bool load(std::string fileName, std::string id, SDL_Renderer *renderer);
	/// Unload an existed texture
	bool unload(std::string id);
	/// Draw target texture
	void draw(std::string id, int x, int y, int width, int height, SDL_Renderer *renderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
	/// Draw selected row * frame of target texture
	void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer *renderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
	/// Draw rotated selected row * frame of target texture
	void drawFrameRotate(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, double angle, SDL_Point* center, SDL_Renderer *renderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
};
