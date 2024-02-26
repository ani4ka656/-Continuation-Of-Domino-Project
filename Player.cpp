#include "Player.h"
int countr = 0;
Player::Player(bool turn_, vector<Tile> tiles_, bool win, int countTiles) : turn(0), tilesOfPlayer(tiles_), win(win), countOfTiles(countTiles) {}

Player::Player() {
    countOfTiles = 0;
    win = false;
    turn = false;
}

void Player::changeWinStatus(bool ifWon)
{
    win = ifWon;
}

void Player::changeTurnStatus(bool shouldBeTurned)
{
    turn = shouldBeTurned;
}

void Player::turnTilesIfNotInTurn(bool notInTurn)
{
    for (int i = 0; i < tilesOfPlayer.size(); i++)
    {
         tilesOfPlayer.at(i).turn(notInTurn);
    }
}

bool Player::inTurn()
{
    return turn;
}

int Player::getCount()
{
    return this->countOfTiles;
}

void Player::setCountOfTiles(int num)
{
    this->countOfTiles = num;
}

void Player::createTiles(int numberOfPictures, vector<string> namesOfPictures, int width, int height)
{   
    // in here first we create all possible combinations of all pictures
    int num = 0;
    for (int i = 0; i < numberOfPictures; i++) 
    {
        string pic1;
        pic1 = namesOfPictures.at(i);
        for (int j = i; j < numberOfPictures; j++) 
        {
            string pic2 = namesOfPictures.at(j);
            Tile t;
            t.set(pic1, pic2, countr, width, height);
            tilesOfPlayer.push_back(t);
            countr++;
        }
    }
    countOfTiles = tilesOfPlayer.size();
}

vector<Tile>& Player::getTiles()
{
    return tilesOfPlayer;
}

void Player::shuffleTiles()
{
    //here from all possible combinations, first we shuffle them, then take only 10 according to the difficulty, that is for now
    default_random_engine seed(std::chrono::system_clock::now().time_since_epoch().count());
    srand(time(0));
    shuffle(tilesOfPlayer.begin(), tilesOfPlayer.end(), seed);
    cout << tilesOfPlayer.size() << endl;
    vector<Tile> newTiles;
    for (int i = 0; i < 10; i++)
    {
        // then take only 10 again by getting random tile out of all of the shuffled combinations
        newTiles.push_back(tilesOfPlayer[rand() % tilesOfPlayer.size()]);
    }
    //like this we can make sure that every time they are different
    cout << newTiles.size() << endl;
    tilesOfPlayer.clear();
    tilesOfPlayer = newTiles;
    countOfTiles = tilesOfPlayer.size();
    cout << tilesOfPlayer.size() << endl;
    for (int i = 0; i < tilesOfPlayer.size(); i++)
    {
        tilesOfPlayer.at(i).print();
    }
}

void Player::addTileToTable(Tile& tile) {
    tilesOfPlayer.push_back(tile);
    countOfTiles++;
}

void Player::addTileToTableFirst(Tile& tile) {
    tilesOfPlayer.insert(tilesOfPlayer.begin(), tile);
    countOfTiles++;
}

void Player::createFirstTileForTable(int numberOfPictures, vector<string> namesOfPictures, int width, int height)
{
    //in here we will make the 1st card for the table, that is actually a player object
    //we make it like how we make the tiles for the player
    int num = 0;
    for (int i = 0; i < numberOfPictures; i++) 
    {
        string pic1;
        pic1 = namesOfPictures.at(i);
        for (int j = i; j < numberOfPictures; j++) {
            string pic2 = namesOfPictures.at(j);
            Tile t;
            t.set(pic1, pic2, countr, width, height);
            tilesOfPlayer.push_back(t);
            countr++;
        }
    }
    default_random_engine seed(std::chrono::system_clock::now().time_since_epoch().count());
    srand(time(0));
    shuffle(tilesOfPlayer.begin(), tilesOfPlayer.end(), seed);
    //but here we only leave the first one
    tilesOfPlayer.resize(1);
    countOfTiles = (int)tilesOfPlayer.size();
}

void Player::setTilesPositions(int x, int y)
{
    int tileWidth = 45;
    int tileHeight = 29;
    for (int i = 0; i < tilesOfPlayer.size(); i++)
    {
        tilesOfPlayer.at(i).changePosition(x, y);
        x += (tileWidth + 5);
    }
}

void Player::removeTile(int pos)
{
    if (tilesOfPlayer.size() != 0)
    {
        tilesOfPlayer.erase(tilesOfPlayer.begin() + pos);
        countOfTiles--;
    }
}

void Player::deleteAllTilesAndPlayerData()
{
    tilesOfPlayer.clear();
    countOfTiles = 0;
    turn = false;
    win = false;
}
