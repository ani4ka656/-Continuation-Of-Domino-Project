#pragma once
#include "Tile.h"
#include <random>   
#include <chrono>    

class Player{
private:
	bool turn;
	bool win;
	int countOfTiles;//the number of til es will be set from the game class because of difficulty 
	vector<Tile> tilesOfPlayer;
public:
	Player();
	Player(bool, vector<Tile>, bool, int);
	void changeWinStatus(bool); // will get 0 or 1
	void changeTurnStatus(bool); // will be 0 or 1
	void turnTilesIfNotInTurn(bool = false);//
	void moveTile(int); //here we remove the tile from the tiles and decrease countOftiiles and we check if we can put tile to each side //proverka za posledna plochka == na nqkoq ot stranite
	bool inTurn(); //returns is it player turn
	int getCount(); //count of each players tiles
	void setCountOfTiles(int);
	void createTiles(int, vector<string>, int, int);
	void createFirstTileForTable(int, vector<string>, int, int); //when creating table, we need a random tile in the middle
	void addTileToTable(Tile&);
	void addTileToTableFirst(Tile&); // when adding tiles to table that is avtually of class player, and we need to add int the begginning
	/// <summary>
	/// we use it to shuffle the tiles
	/// </summary>
	void shuffleTiles(); 
	vector<Tile>& getTiles();
	void setTilesPositions(int, int);
	void removeTile(int);
	void deleteAllTilesAndPlayerData();

};

