#include "TextureManager.h"

bool TextureManager::loadTexture(const char* fileName, std::string id, SDL_Renderer* ren) {
	SDL_Surface* tempSurface = IMG_Load(fileName);

	if (tempSurface == 0) //somethig went wrong
		return false;
	SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, tempSurface);

	if (texture != 0) {
		textureMap[id] = texture;
		return true;
	}
	return false; //someting went wrong 

}

//bool loadFont(TTF_Font* font, std::string id, SDL_Renderer* ren, std::string text, SDL_Rect rect){
//SDL_Surface* tempSurfaceText = NULL;
//
//tempSurfaceText = TTF_RenderText_Blended(font, text.c_str() , { 0, 0, 255, 255 });
////SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, tempSurfaceText);
/////texture = SDL_CreateTextureFromSurface(ren, tempSurfaceText);
//
//	//if (tempSurfaceText == 0) //somethig went wrong
//	//return false;
//	SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, tempSurfaceText);
//	int tw, th;
//	// the texture created from the text has certain width and height,
//	// get that width and height and give it to the destination rectangle
//	// so the text does not appear weird and stretched on the window
//	
//	if (tempSurfaceText == 0) //somethig went wrong
//		return false;
//if (texture != 0) {
//
//	//TextureManager::Instance()->textureMap[id] = texture;
//	textureMap[id] = texture;
//	SDL_QueryTexture(texture, 0, 0, &tw, &th);
//	rect = { rect.x, rect.y, tw, th };
//	return true;
//}
//SDL_FreeSurface(tempSurfaceText);
//return false; //someting went wrong 
//
//}

void TextureManager::drawTile(int tileId, std::string idPic1, std::string idPic2, int x, int y, Tile tile, int width, int height, SDL_Renderer* ren, SDL_RendererFlip flip, float angle) {
	tile.changePosition(x, y, angle);
	SDL_Rect srcRect;
	srcRect.x = srcRect.y = 0;
	int picWidth = 45;
	int picHeight = 29;
	srcRect.w = picWidth;
	srcRect.h = picHeight;
	
	//cout << "x: " << tile.tileParent.x << " y: " << tile.tileParent.y << endl;
	//cout << "x: " << x << " y: " << y << endl;
	//cout << tile.getPosition().first << endl;
	SDL_SetRenderDrawColor(ren, 153, 255, 204, 255);
	//SDL_RenderClear(ren);
	SDL_RenderFillRect(ren, &tile.tileParent);
	/*cout << "angle: " << tile.angle << endl;*/
	SDL_RenderCopyEx(ren, textureMap[idPic1], &srcRect, &tile.destPic1, tile.angle , 0, flip);
	SDL_RenderCopyEx(ren, textureMap[idPic2], &srcRect, &tile.destPic2, tile.angle, 0, flip);

}

void TextureManager::drawTexture(std::string id, int x, int y, int width, int height, SDL_Renderer* ren, SDL_RendererFlip flip) {
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = srcRect.y = 0;
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;
	SDL_RenderCopyEx(ren, textureMap[id], &srcRect, &destRect, 0, 0, flip);

}

void TextureManager::drawOneFrameFromTexture(std::string id,
	int x, int y, //window x, y, to draw at;
	int width, int height, //width and height of texture
	int currentRow, int currentFrame, //row starts from 1
	SDL_Renderer* ren, 
	SDL_RendererFlip flip){//flag to flip texture
	SDL_Rect srcRect;
	SDL_Rect destRect;
	srcRect.x = width * currentFrame;
	srcRect.y = height * (currentRow - 1);
	srcRect.w = destRect.w = width;
	srcRect.h = destRect.h = height;
	destRect.x = x;
	destRect.y = y;
	SDL_RenderCopyEx(ren, textureMap[id], &srcRect, &destRect, 0, 0, flip);
}

TextureManager* TextureManager::instance = 0;