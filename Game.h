#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "TextureManager.h"
#include "Tile.h"
#include "Player.h"
class Game {
public:
	Game();
	~Game();
	bool init(const char* title, int xpos, int ypos, int width, int height, int flags);
	bool ttf_init();

	bool isTileClicked(SDL_Rect* rect, int xDown, int yDown, int xUp, int yUp);
	bool isClickableTextureClicked(SDL_Texture* t, SDL_Rect* r, int xDown, int yDown, int xUp, int yUp);
	void moveTileToTableAbove(Player& player, int position, int side, float rotationOfTile, int changeX, int changeY);
	void moveTileToTableBelow(Player& player, int position, int side, float rotationOfTile, int changeX, int changeY);
	void reloadGame();
	void render();
	/*bool isClickedBesideTileWithRot(int x, int y, Tile tile);
	bool isClickedAboveTileWithRot(int x, int y, Tile tile, int i);
	bool isClickedNextToTileWithRot(int x, int y, Tile tile);
	bool isClickedBelowTileWithRot(int x, int y, Tile tile, int i);*/
	bool isClickedBesideTile(int x, int y, Tile tile);
	bool isClickedAboveTile(int x, int y, Tile tile);
	bool isClickedNextToTile(int x, int y, Tile tile);
	bool isClickedBeside2Tile(int x, int y, Tile tile);
	bool isClickedBelowTile(int x, int y, Tile tile);
	bool isClickedNext2ToTile(int x, int y, Tile tile);

	void update();
	void handleEvents();
	void clean();
	bool isRunning();
	bool test();

private:
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	bool running;
	Player player1;
	Player player2;
	Player table;
	int difficulty;
	vector<string> ids;
	int mouseDownX, mouseDownY;
	int xPosT, yPosT;
	int tileWidth;
	int tileHeight;
	SDL_Rect destGrid;
	SDL_Rect destPlayer1;
	SDL_Rect destPlayer2;
	int selectedTile;
	SDL_Rect destMenu;
	SDL_Rect destTheme;
	SDL_Rect destNewGame;
	SDL_Rect destGameMenu;
	SDL_Rect destPass;
	SDL_Rect destGame;
	bool menuClicked;
	bool passClicked;
	bool newGameClicked;
	bool themeClicked;
	vector<bool> visibility;
	SDL_Texture* menuNonClickable;
	SDL_Texture* themeClickable;
	SDL_Texture* newGameClickable;
	SDL_Texture* menuClickable;
	SDL_Texture* passClickable;
	
	

};