#pragma once
#include "Game.h"
#include "Tile.h"
#include <string>
#include <map>
#include <SDL_ttf.h>
class TextureManager
{
public:

	bool loadTexture(const char* filename, std::string id, SDL_Renderer* ren);
	//bool loadFont(TTF_Font* font, std::string id, SDL_Renderer* ren, std::string text, SDL_Rect rect);
	void drawTile(int tileId, std::string idPic1, std::string idPic2,
		int x, int y,
		Tile tile,
		int width, int height,
		SDL_Renderer* ren,
		SDL_RendererFlip = SDL_FLIP_NONE, float angle = 0);
	void drawTexture(std::string id,
					int x, int y,
					int width, int height, 
					SDL_Renderer* ren, 
					SDL_RendererFlip = SDL_FLIP_NONE);
	void drawOneFrameFromTexture(std::string id,
		int x, int y,
		int width, int height,
		int currentRow, int currentFrame,
		SDL_Renderer* ren,
		SDL_RendererFlip flip = SDL_FLIP_NONE);
	static TextureManager* Instance() {
		if (instance == 0) {
			instance = new TextureManager();
			return instance;
		}
		return instance;
	}
private:
	std::map<std::string, SDL_Texture*> textureMap;
	TextureManager(){}
	static TextureManager* instance;
};

