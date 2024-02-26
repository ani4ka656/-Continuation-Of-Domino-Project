#include "Tile.h"
Tile::Tile() {
	position.first = 0;
	position.second = 0;
	tileWidth = 0;
	tileHeight = 0;
	tileParent = { 0, 0, tileWidth, tileHeight };
	destPic1 = { tileParent.x, tileParent.y, tileWidth, tileHeight / 2 };
	destPic2 = { tileParent.x, tileParent.y + tileHeight / 2, tileWidth , tileHeight / 2 };
	angle = 0;
	id = 0;
	turned = false;
}

void Tile::set(string pic1, string pic2, int id, int width, int height) {
	this->firstPic.id = pic1;
	this->secondPic.id = pic2;
	this->firstPic.sides = { 1, 1, 1 };
	this->secondPic.sides = { 1, 1, 1 };
	turned = false;
	position.first = 0;
	position.second = 0;
	id = id;
	tileWidth = width;
	tileHeight = height;
}

void Tile::turn(bool turn) {
	this->turned = turn;
}

void Tile::changeAvailableSide(string id, int place) {
	if (firstPic.id == id)
		firstPic.sides.at(place) = 0;
	if (secondPic.id == id)
		secondPic.sides.at(place) = 0;
}

void Tile::print() const {
	cout << "first pic id:" << firstPic.id << endl;
	cout << "second pic id:" << secondPic.id << endl;
	cout << "Tile_rectx:" << tileParent.x << endl;
	cout << "Tile_recty:" << tileParent.y << endl;
	cout << "tile turned:" << turned << endl << endl << endl;
}

void Tile::changePosition(int x, int y, float a) {
	this->position.first = x;
	this->position.second = y;
	angle = a;
	//Here we rotate the tile avvording to the angle by rotating first the tileRect and then changing the tiles
	if (angle == 0) {
		tileParent = { position.first, position.second , tileWidth, tileHeight };
		destPic1 = { tileParent.x + 3, tileParent.y + 3, tileWidth - 6, tileHeight / 2 - 6 };
		destPic2 = { tileParent.x + 3, (tileParent.y + tileHeight / 2) + 3, tileWidth - 6, tileHeight / 2 - 6 };
	}
	else if (angle == 90 ) {
		tileParent = { position.first, position.second , tileHeight, tileWidth };
		destPic1 = { tileParent.x + 3, tileParent.y + 3, tileHeight / 2 - 6, tileWidth - 6 };
		destPic2 = { destPic1.x + destPic1.w + 6, destPic1.y, tileWidth - 6, tileHeight / 2 - 6 };
	}
	else if (angle == -90) {
		tileParent = { position.first, position.second, tileHeight,  tileWidth };
		destPic1 = { tileParent.x + 3 , tileParent.y + 3, tileHeight / 2 - 6 , tileWidth - 6 };
		destPic2 = { tileParent.x + 3 + tileHeight / 2, tileParent.y + 3, tileHeight / 2 - 6, tileWidth - 6 };
	}
	else if (angle == 180 || angle == -180) {
		tileParent = { position.first, position.second ,tileWidth, tileHeight };
		destPic1 = { tileParent.x + 3 , (tileParent.y + tileHeight / 2) + 3, tileWidth - 6, tileHeight / 2 - 6 };
		destPic2 = { tileParent.x + 3, tileParent.y + 3, tileWidth - 6, tileHeight / 2 - 6 };
	}
	else if (angle == -270) {
		tileParent = { position.first, position.second, tileHeight ,tileWidth };
		destPic1 = { tileParent.x + 3, tileParent.y + 3, tileHeight / 2 - 6 , tileWidth - 6 };
		destPic2 = { (tileParent.x + tileHeight / 2) + 3, tileParent.y + 3, tileHeight / 2 - 6,  tileWidth - 6 };
	}
	else if (angle == 270) {
		tileParent = { position.first, position.second, tileHeight,tileWidth };
		destPic1 = { tileParent.x + 3 + tileHeight / 2, tileParent.y + 3  , tileHeight / 2 - 6, tileWidth - 6 };
		destPic2 = { tileParent.x + 3 , tileParent.y + 3,  tileHeight / 2 - 6, tileWidth - 6 };
	}
}

string Tile::getPictureid1(){
	return firstPic.id;
}

string Tile::getPictureid2() {
	return secondPic.id;

}
pair<int, int> Tile::getPosition() {
	return this->position;
}

int Tile::getTileId() {
	return this->id;
}

vector<bool> Tile::getSideAvailabilityOfPic(int i) {
	if (i == 1)
		return firstPic.sides;
	else
		return secondPic.sides;
}

//it will be used when we connect tiles to one another
void Tile::changeSideAvailability(int side, int pic) {
	if (pic == 1) 
		firstPic.sides.at(side) = 0;
	else
		secondPic.sides.at(side) = 0;
}