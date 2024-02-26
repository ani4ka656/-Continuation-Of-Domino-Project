#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <ctime>
#include <stdlib.h>    
#include <algorithm>    
#include <utility>    
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
using namespace std;

struct Picture {
	string id;
	vector<bool> sides;
};

class Tile {
private:
	Picture firstPic;
	Picture secondPic;
	bool turned;
	pair<int, int> position;//Point(x,y)
	int id;
	int tileWidth;
	int tileHeight;
public:
	SDL_Rect destPic1;
	SDL_Rect destPic2;
	SDL_Rect tileParent;
	float angle;
	Tile();
	void set(string, string, int, int, int);
	void turn(bool);
	void changeAvailableSide(string, int);
	void print() const;
	void changePosition(int, int, float = 0);
	string getPictureid1();
	string getPictureid2();
	pair<int, int> getPosition();
	int getTileId();
	vector<bool> getSideAvailabilityOfPic(int);
	void changeSideAvailability(int, int);
};//end class

