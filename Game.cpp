#include "Game.h"

bool Game::init(const char* title, int xpos, int ypos, int width, int height, int flags) {
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "SDL init success\n";

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window != 0) { //window init success
			std::cout << "window creation success\n";
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer != 0) { //renderer init success
				std::cout << "renderer creation success\n";

				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

				ids = { "1.jpeg", "2.jpeg", "3.jpeg", "4.jpeg", "5.jpeg",
						"6.jpeg", "7.jpeg", "8.jpeg", "9.jpeg", "10.jpeg",
						"11.jpeg", "12.jpeg", "13.jpeg", "14.jpeg", "15.jpeg" };

				difficulty = 10;
				//loading the textures from the vector of ids for the pictures
				for (int i = 0; i < difficulty; i++) {
					string namePic = ids[i];
					string path = "assets/cars/" + namePic;
					TextureManager::Instance()->loadTexture(path.c_str(), ids[i], renderer);
				}

				//creating the tiles for player 1 and shuffling them
				player1.createTiles(difficulty, ids, 45, 90);
				player1.shuffleTiles();

				//creating the tiles for player 2 and shuffling them
				player2.createTiles(difficulty, ids, 45, 90);
				player2.shuffleTiles();

				//creating the tile for the table
				table.createFirstTileForTable(10, ids, 45, 90);

				int ww, wh;

				tileWidth = 45;
				tileHeight = 90;

				//after we figure out the problem with recalculating the four sides we will need them to limit placing of the tiles
				destGrid = { 0, 100, 900, 450 };
				destPlayer1 = { 0, 0, 900, 100 };
				destPlayer2 = { 550, 0, 900, 100 };

				SDL_GetWindowSize(window, &ww, &wh);

				xPosT = (destGrid.x + destGrid.w) / 2 - 45;
				yPosT = destGrid.y + (destGrid.h / 2) - 45;

				//placing the first tile in the middle
				table.getTiles().at(0).changePosition(xPosT, yPosT);

				// here we set initial player turn
				player1.changeTurnStatus(1);
				player2.changeTurnStatus(0);
			}
			else {
				cout << "renderer init failed\n";
				return false;
			}
		}
		else {
			cout << "window init failed\n";
			return false;
		}
	}
	else {
		cout << "SDL init fail\n";
		return false;
	}
	cout << "init success\n";
	running = true;
	return true;
}


bool Game::ttf_init() {
	if (TTF_Init() == -1) { // If the initialization of ttf fails, return false
		return false;
	}

	TTF_Font* font1 = TTF_OpenFont("fonts/Arcade.ttf", 11);
	TTF_Font* font2 = TTF_OpenFont("fonts/segoepr.ttf", 50);
	TTF_Font* font3 = TTF_OpenFont("fonts/segoepr.ttf", 25);


	if (font1 == NULL || font2 == NULL) { // return false if either font fails to load
		return false;
	}
	int ww, wh;

	SDL_GetWindowSize(window, &ww, &wh);

	visibility = { 1, 1, 0, 0, 0 };

	//in here we create the text surfaces and set their destRectangles
	SDL_Surface* tempSurfaceText = NULL;

	tempSurfaceText = TTF_RenderText_Blended(font2, "Menu", { 0, 0, 255, 255 });
	menuNonClickable = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	tempSurfaceText = TTF_RenderText_Blended(font2, "Theme", { 255, 0, 0, 255 });
	themeClickable = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	tempSurfaceText = TTF_RenderText_Blended_Wrapped(font3, "New Game",
		{ 0, 255, 0, 255 }, 300);
	newGameClickable = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	tempSurfaceText = TTF_RenderText_Blended_Wrapped(font3, "Menu", { 255, 0, 255, 255 }, 500);
	menuClickable = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	tempSurfaceText = TTF_RenderText_Blended(font3, "Pass", { 0, 0, 0, 255 });
	passClickable = SDL_CreateTextureFromSurface(renderer, tempSurfaceText);

	int tw, th;
	SDL_QueryTexture(menuNonClickable, 0, 0, &tw, &th);
	destMenu = { ww / 2, 5, tw, th };

	SDL_QueryTexture(themeClickable, 0, 0, &tw, &th);
	destTheme = { 20, 100,tw, th };

	SDL_QueryTexture(newGameClickable, 0, 0, &tw, &th);
	destNewGame = { 0, 0, tw, th };

	SDL_QueryTexture(menuClickable, 0, 0, &tw, &th);
	destGameMenu = { 0, 20, tw, th };

	SDL_QueryTexture(passClickable, 0, 0, &tw, &th);
	destPass = { 0, 40,tw, th };

	SDL_FreeSurface(tempSurfaceText);
	TTF_CloseFont(font1);
	TTF_CloseFont(font2);

	return true;
}

void Game::render() {


	SDL_SetRenderDrawColor(renderer, 229, 255, 204, 255);
	SDL_RenderClear(renderer);
	//if theme is not clicked, show only the menu
	if (themeClicked == 0) {

		SDL_RenderCopy(renderer, menuNonClickable, NULL, &destMenu);
		SDL_RenderCopy(renderer, themeClickable, NULL, &destTheme);
	}
	//if menu is clicked inside the game
	else if (menuClicked && visibility.at(0) == 1 && visibility.at(1) == 1) {

		SDL_RenderCopy(renderer, menuNonClickable, NULL, &destMenu);
		SDL_RenderCopy(renderer, themeClickable, NULL, &destTheme);

	}
	//if pass is clicked its the other player turn
	else if (passClicked && visibility.at(0) == 0 && visibility.at(1) == 0) {

		if (player1.inTurn() == 1) {
			player1.changeTurnStatus(0);
			player2.changeTurnStatus(1);
		}
		else {
			player1.changeTurnStatus(1);
			player2.changeTurnStatus(0);
		}
		passClicked = 0;
	}
	//if we chose the theme then the game starts, or if we chose the new game, it reloads
	if (themeClicked == 1 || newGameClicked == 1 && visibility.at(0) == 0 && visibility.at(1) == 0) {
		int ww, wh;

		SDL_RenderCopy(renderer, menuClickable, NULL, &destGameMenu);
		SDL_RenderCopy(renderer, newGameClickable, NULL, &destNewGame);
		SDL_RenderCopy(renderer, passClickable, NULL, &destPass);

		SDL_GetWindowSize(window, &ww, &wh);

		//creating the to lines dividing player 1 and player 2 with the grid
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		SDL_RenderDrawLine(renderer, 0, 100, ww, 100);
		SDL_RenderDrawLine(renderer, 0, 550, ww, 550);

		//drawing the board lines
		for (int i = 0; i < 10; i++) {
			SDL_RenderDrawLine(renderer, 0, (i * 45) + 100, ww, (i * 45) + 100);
		}
		for (int i = 0; i < 20; i++) {
			SDL_RenderDrawLine(renderer, i * 45, 100, i * 45, wh - 100);
		}

		int offsetXPlayer1 = 150;
		int offsetYPlayer1 = 5;
		int offsetXPlayer2 = 50;
		int offsetYPlayer2 = 555;
		int paddingXPlayer1 = 20;
		int paddingXPlayer2 = 20;
		int multiplyBy1 = 0;
		int multiplyBy2 = 0;

		//drawing the tiles of player 1
		for (int i = 0; i < player1.getCount(); i++) {

			int x = multiplyBy1 * (45 + paddingXPlayer1) + offsetXPlayer1;
			int y = offsetYPlayer1;

			player1.getTiles().at(i).changePosition(x, y);

			string firstPictureTileId = player1.getTiles().at(i).getPictureid1();
			string secondPictureTileId = player1.getTiles().at(i).getPictureid2();
			TextureManager::Instance()->drawTile(i, firstPictureTileId, secondPictureTileId, x, y, player1.getTiles().at(i), tileWidth, tileHeight, renderer);
			multiplyBy1++;
		}

		//drawing the tiles of player 2 
		for (int i = 0; i < player2.getCount(); i++) {

			int x = multiplyBy2 * (45 + paddingXPlayer2) + offsetXPlayer2;
			int y = offsetYPlayer2;

			player2.getTiles().at(i).changePosition(x, y);
			string firstPictureTileId = player2.getTiles().at(i).getPictureid1();
			string secondPictureTileId = player2.getTiles().at(i).getPictureid2();

			TextureManager::Instance()->drawTile(i, firstPictureTileId, secondPictureTileId, x, y, player2.getTiles().at(i), 45, 90, renderer);
			multiplyBy2++;
		}

		//drawing the lines of the table
		for (int i = 0; i < table.getCount(); i++) {

			string pic1Id = table.getTiles().at(i).getPictureid1();
			string pic2Id = table.getTiles().at(i).getPictureid2();

			int x = table.getTiles().at(i).getPosition().first;
			int y = table.getTiles().at(i).getPosition().second;

			TextureManager::Instance()->drawTile(1, pic1Id, pic2Id, x, y, table.getTiles().at(i), 45, 90, renderer, SDL_FLIP_NONE, table.getTiles().at(i).angle);
		}
	}
	SDL_RenderPresent(renderer);
}

void Game::handleEvents() {
	SDL_Event event;
	//int selectedPlayer = 0;

	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT: running = false; break;
		case SDL_MOUSEBUTTONDOWN: {
			int msx, msy;

			if (event.button.button == SDL_BUTTON_LEFT) {
				int ww, wh;
				SDL_GetWindowSize(window, &ww, &wh);
				SDL_GetMouseState(&msx, &msy);
				//std::cout << "mouse button left\n";
				int quadrantWidth = 25;
				int quadrantHeight = wh / 2;
				mouseDownX = msx;
				mouseDownY = msy;
				for (int i = 0; i < player1.getCount(); i++) {

					int x = player1.getTiles().at(i).tileParent.x;
					int y = player1.getTiles().at(i).tileParent.y;
					int widthTile = player1.getTiles().at(i).tileParent.w;
					int heightTile = player1.getTiles().at(i).tileParent.h;


					//in here we check if we clicked on one of player 1 tiles and get its index
					if (isTileClicked(&player1.getTiles().at(i).tileParent, mouseDownX, mouseDownY, msx, msy)) {
						selectedTile = i;
					}

				}
				for (int i = 0; i < player2.getCount(); i++) {

					int x = player2.getTiles().at(i).tileParent.x;
					int y = player2.getTiles().at(i).tileParent.y;
					int widthTile = player2.getTiles().at(i).tileParent.w;
					int heightTile = player2.getTiles().at(i).tileParent.h;

					//in here we check if we clicked on one of player 2 tiles and get its index
					if (isTileClicked(&player2.getTiles().at(i).tileParent, mouseDownX, mouseDownY, msx, msy)) {
						selectedTile = i;
					}
				}
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				// if right button is clicked
				int ww, wh;
				SDL_GetWindowSize(window, &ww, &wh);
				SDL_GetMouseState(&msx, &msy);
				mouseDownX = msx;
				mouseDownY = msy;
				//in here we check if we clicked on either side of the tile, it doesn't work after rotation
				for (int i = 0; i < table.getCount(); i++) {
					cout << (isClickedBesideTile(mouseDownX, mouseDownY, table.getTiles().at(i)) ? "Clicked Baseide : yes" : "Clicked Baseide : Not") << endl;
					cout << (isClickedAboveTile(mouseDownX, mouseDownY, table.getTiles().at(i)) ? "Clicked Above : yes" : "Clicked Above : Not") << endl;
					cout << (isClickedNextToTile(mouseDownX, mouseDownY, table.getTiles().at(i)) ? "Clicked Next: yes" : "Clicked Next: Not") << endl;
					cout << (isClickedBeside2Tile(mouseDownX, mouseDownY, table.getTiles().at(i)) ? "Clicked Baseide2 : yes" : "Clicked Baseide2 : Not") << endl;
					cout << (isClickedBelowTile(mouseDownX, mouseDownY, table.getTiles().at(i)) ? "Clicked Below : yes" : "Clicked Below : Not") << endl;
					cout << (isClickedNext2ToTile(mouseDownX, mouseDownY, table.getTiles().at(i)) ? "Clicked next2: yes" : "Clicked Next2: Not") << endl;
				}

				int last = (int)table.getCount() - 1;

				cout << "Index:" << selectedTile << endl;
				cout << "cntr:" << last << endl;

				cout << "Player in turn: 1" << endl;
				

				//in here we need to check if the tile we chose and if after clicking with the right button, we can put it on all sides of the tile that is the first
				//we only need to check if it can be placed on the front and the back, accoding to the assignment
				//but it only works for now for tiles placed above or below the first tile, because after every rotatiton the sides:
				// above, below, next, next2, beside and beside 2 change for  each tile
				//if (player2.inTurn() == 1) {
				float anglebegin = table.getTiles().at(0).angle;
				cout << "angle: "  << anglebegin << endl;
				//float angleLast = table.getTiles().at(last).angle;
				//if (player1.inTurn() == 1) {
				//int realXabove = 0;
				//int realYabove = 0;
				//int realXbeside = 0;
				//int realYbeside = 0;
				//int realXbeside2 = 0;
				//int realYbeside2 = 0;
				//int realXnext = 0;
				//int realYnext = 0;
				//int realXnext2 = 0;
				//int realYnext2 = 0;
				//int realXbelow = 0;
				//int realYbelow = 0;

				//cout << "Player in turn: 1" << endl;

				//if (anglebegin == 0 || anglebegin == 180) {
				//	realXabove = msx;
				//	realYabove = msy;
				//	realXbeside = msx;
				//	realYbeside = msy;
				//	realXbeside2 = msx;
				//	realYbeside2 = msy;
				//	realXnext = msx;
				//	realYnext = msy;
				//	realXnext2 = msx;
				//	realYnext2 = msy;
				//	realXbelow = msx;
				//	realYbelow = msy;
				//}/*if (anglebegin == -90) {
				//	realXabove = msx + 90;
				//	realYabove = msy - 45;
				//	realXbeside = msx + 45;
				//	realYbeside = msy - 45;
				//	realXbeside2 = msx;
				//	realYbeside2 = msy;
				//	realXnext = msx + 45;
				//	realYnext = msy - 90;
				//	realXnext2 = msx - 45;
				//	realYnext2 = msy - 45;
				//	realXbelow = msx;
				//	realYbelow = msy + 45;
				//}*/ else if (anglebegin == 90 || anglebegin == 270) {
				//	realXabove = msx - 90;
				//	realYabove = msy + 45;
				//	realXbeside = msx - 45;
				//	realYbeside = msy + 90;
				//	realXbeside2 = msx - 45;
				//	realYbeside2 = msy + 45;
				//	realXnext = msx - 90;
				//	realYnext = msy - 45;
				//	realXnext2 = msx - 45;
				//	realYnext2 = msy - 90;
				//	realXbelow = msx + 45;
				//	realYbelow = msy - 90;
				//}if (anglebegin == -270 || anglebegin == -90) {
				//	realXabove = msx + 135;
				//	realYabove = msy;
				//	realXbeside = msx + 90;
				//	realYbeside = msy - 45;
				//	realXbeside2 = msx + 45;
				//	realYbeside2 = msy - 90;
				//	realXnext = msx + 45;
				//	realYnext = msy + 90;
				//	realXnext2 = msx;
				//	realYnext2 = msy + 45;
				//	realXbelow = msx - 45;
				//	realYbelow = msy - 90;
				//}
				//cout << "relaX: " << realXabove << "relaY: " << realYabove << endl;
				//cout << "msx: " << msx << "msy: " << msy << endl;
				//in here we need to check if the tile we chose and if after clicking with the right button, we can put it on all sides of the tile that is the first
				//we only need to check if it can be placed on the front and the back, accoding to the assignment
				//but it only works for now for tiles placed above or below the first tile, because after every rotatiton the sides:
				// above, below, next, next2, beside and beside 2 change for  each tile
				if (isClickedAboveTile(msx, msy, table.getTiles().at(0)))
				{
					//cout << "Index:" << selectedTile << endl;
					moveTileToTableAbove(player1, selectedTile, 2, 0, 0, -90);
				}
				else if (isClickedBelowTile(msx, msy, table.getTiles().at(0))) {

					moveTileToTableBelow(player1, selectedTile, 2, 0, 0, 90);
				}
				else if (isClickedBesideTile(msx, msy, table.getTiles().at(0))) {

					moveTileToTableAbove(player1, selectedTile, 1, 90.0f, -90, 0);
				}
				else if (isClickedBeside2Tile(msx, msy, table.getTiles().at(0))) {

					moveTileToTableBelow(player1, selectedTile, 3, 90.0f, -90, +45);
				}
				else if (isClickedNext2ToTile(msx, msy, table.getTiles().at(0))) {

					moveTileToTableBelow(player1, selectedTile, 2, -270.0f, 45, 45);
				}
				else if (isClickedNextToTile(msx, msy, table.getTiles().at(0))) {

					moveTileToTableAbove(player1, selectedTile, 3, -270.0f, 45, 0);
				}
				//if the table has more than one tile we need to check if we can place at the bottom and it tree sides
				if (last >= 1) {
					/*if (isClickedAboveTile(msx, msy, table.getTiles().at(last)))
					{
						cout << "Index:" << selectedTile << endl;
						moveTileToTableAbove(player1, selectedTile, 2, 0, 0, -90);
					}
					else */if (isClickedBelowTile(msx, msy, table.getTiles().at(last))) {

						moveTileToTableBelow(player1, selectedTile, 2, 0, 0, 90);
					}
					/*else if (isClickedBesideTile(msx, msy, table.getTiles().at(last))) {

						moveTileToTableAbove(player1, selectedTile, 1, 90.0f, -90, 0);
					}*/
					else if (isClickedBeside2Tile(msx, msy, table.getTiles().at(last))) {

						moveTileToTableBelow(player1, selectedTile, 3, 90.0f, -90, +45);
					}
					else if (isClickedNext2ToTile(msx, msy, table.getTiles().at(last))) {

						moveTileToTableBelow(player1, selectedTile, 2, -270.0f, 45, 45);
					}
					/*else if (isClickedNextToTile(msx, msy, table.getTiles().at(last))) {

						moveTileToTableAbove(player1, selectedTile, 3, -90.0f, 45, 0);
					}*/
				}


				//this here is for when we resolve the problems
				//we just change the turn and it is now the second player turn
					//player1.changeTurnStatus(0);
					//player2.changeTurnStatus(1);
				//}
				//else if (player2.inTurn() == 1) {
				//	cout << "Player in turn: 1" << endl;
				//	if (isClickedAboveTile(msx, msy, table.getTiles().at(0)))
				//	{
				//		//cout << "Index:" << selectedTile << endl;
				//		moveTileToTableAbove(player2, selectedTile, 2, 0, 0, -90);
				//	}
				//	else if (isClickedBelowTile(msx, msy, table.getTiles().at(0))) {

				//		moveTileToTableBelow(player2, selectedTile, 2, 0, 0, 90);
				//	}
				//	else if (isClickedBesideTile(msx, msy, table.getTiles().at(0))) {

				//		moveTileToTableAbove(player2, selectedTile, 1, 90.0f, -90, 0);
				//	}
				//	else if (isClickedBeside2Tile(msx, msy, table.getTiles().at(0))) {

				//		moveTileToTableBelow(player2, selectedTile, 3, 90.0f, -90, +45);
				//	}
				//	else if (isClickedNext2ToTile(msx, msy, table.getTiles().at(0))) {

				//		moveTileToTableBelow(player2, selectedTile, 2, -270.0f, 45, 45);
				//	}
				//	else if (isClickedNextToTile(msx, msy, table.getTiles().at(0))) {

				//		moveTileToTableAbove(player2, selectedTile, 3, -90.0f, 45, 0);
				//	}
				//	else if (last > 1) {
				//		if (isClickedAboveTile(msx, msy, table.getTiles().at(last)))
				//		{
				//			cout << "Index:" << selectedTile << endl;
				//			moveTileToTableAbove(player2, selectedTile, 2, 0, 0, -90);
				//		}
				//		else if (isClickedBelowTile(msx, msy, table.getTiles().at(last))) {

				//			moveTileToTableBelow(player2, selectedTile, 2, 0, 0, 90);
				//		}
				//		else if (isClickedBesideTile(msx, msy, table.getTiles().at(last))) {

				//			moveTileToTableAbove(player2, selectedTile, 1, 90.0f, -90, 0);
				//		}
				//		else if (isClickedBeside2Tile(msx, msy, table.getTiles().at(last))) {

				//			moveTileToTableBelow(player2, selectedTile, 3, 90.0f, -90, +45);
				//		}
				//		else if (isClickedNext2ToTile(msx, msy, table.getTiles().at(last))) {

				//			moveTileToTableBelow(player2, selectedTile, 2, -270.0f, 45, 45);
				//		}
				//		else if (isClickedNextToTile(msx, msy, table.getTiles().at(last))) {

				//			moveTileToTableAbove(player2, selectedTile, 3, -90.0f, 45, 0);
				//		}
				//		player1.changeTurnStatus(1);
				//		player2.changeTurnStatus(0);
				//	}
				//}
			}
		}; break;
		case SDL_MOUSEBUTTONUP: {
			int msx, msy;
			if (event.button.button == SDL_BUTTON_LEFT) {
				// When the left mouse button is let go, check if the texture was pressed and show whatever is needed 
				SDL_GetMouseState(&msx, &msy);
				if (isClickableTextureClicked(themeClickable, &destTheme, mouseDownX, mouseDownY, msx, msy)) {
					themeClicked = 1;
					visibility = { 0, 0 , 1, 1, 1, 1 };
				}
				if (isClickableTextureClicked(newGameClickable, &destNewGame, mouseDownX, mouseDownY, msx, msy)) {
					newGameClicked = 1;
					reloadGame();
					visibility = { 0, 0 , 1,1, 1, 1 };
				}if (isClickableTextureClicked(menuClickable, &destGameMenu, mouseDownX, mouseDownY, msx, msy)) {
					menuClicked = 1;
					themeClicked = 0;
					passClicked = 0;
					visibility = { 1, 1 , 0, 0, 0, 0 };
				}if (isClickableTextureClicked(passClickable, &destPass, mouseDownX, mouseDownY, msx, msy)) {
					passClicked = 1;
					visibility = { 0, 0 , 1, 1,1, 1 };
				}

			}
		}; break;
		default: break;
		}
	}
}

void Game::update() {

}

bool Game::isClickableTextureClicked(SDL_Texture* t, SDL_Rect* r, int xDown, int yDown, int xUp, int yUp) {
	int tw, th;
	SDL_QueryTexture(t, 0, 0, &tw, &th);


	// checks if the cursor position during the click time is inside the coordinates
	if ((xDown > r->x && xDown < (r->x + tw)) && (xUp > r->x && xUp < (r->x + tw)) &&
		(yDown > r->y && yDown < (r->y + th)) && (yUp > r->y && yUp < (r->y + th))) {
		// if all coordinates fall inside the rendered texture, the texture is clicked
		return true;
	}
	// the click was outside the texture
	return false;
}

bool Game::isTileClicked(SDL_Rect* r, int xDown, int yDown, int xUp, int yUp) {

	// checks if the cursor position during the click time is inside the coordinates of the tile
	if ((xDown > r->x && xDown < (r->x + r->w)) && (xUp > r->x && xUp < (r->x + r->w)) &&
		(yDown > r->y && yDown < (r->y + r->h)) && (yUp > r->y && yUp < (r->y + r->h))) {
		return true;
	}
	return false;
}

//bool Game::isClickedBesideTile(int x, int y, Tile tile) {
//	// checks if the cursor position is on the left side of the first picture of the tile
//	//cout << "x:" << x << " y:" << y << " tileX:"<< tile.tileParent.x << " tileY:" << tile.tileParent.y << endl;
//	if (tile.angle == 0 || tile.angle == 180) {
//		if ((x + tile.tileParent.w) > tile.tileParent.x && y > tile.tileParent.y && (x + tile.tileParent.w) < (tile.tileParent.x + tile.tileParent.w) && y < (tile.tileParent.y + (tile.tileParent.h / 2)))
//			return true;
//		else return false;
//	}else if (tile.angle == 90 || tile.angle == 270) {
//		if ((x + tile.tileParent.w) > tile.tileParent.x && y > tile.tileParent.y && (x + tile.tileParent.w) < (tile.tileParent.x + tile.tileParent.w) && y < (tile.tileParent.y + (tile.tileParent.h / 2)))
//			return true;
//		else return false;
//	}
//}

bool Game::isClickedBesideTile(int x, int y, Tile tile) {
	// checks if the cursor position is on the left side of the first picture of the tile
	//cout << "x:" << x << " y:" << y << " tileX:"<< tile.tileParent.x << " tileY:" << tile.tileParent.y << endl;
	if (tile.angle == 0 || tile.angle == 180) {
		if ((x + tileWidth) > tile.tileParent.x && y > tile.tileParent.y && (x + tileWidth) < (tile.tileParent.x + tileWidth) && y < (tile.tileParent.y + (tileHeight / 2)))
			return true;
		else return false;
	}
	else if (tile.angle == 90 || tile.angle == 270) {
		if ( x  > tile.tileParent.x && y - tileHeight / 2 > tile.tileParent.y && x < (tile.tileParent.x + tileWidth) && y < tile.tileParent.y)
			return true;
		else return false;
	}else if (tile.angle == -90 || tile.angle == -270) {
		if ( x  < tile.tileParent.x && y + tileHeight / 2 > tile.tileParent.y &&  x > (tile.tileParent.x - tileWidth) && y < tile.tileParent.y)
			return true;
		else return false;
	}

}

bool Game::isClickedAboveTile(int x, int y, Tile tile) {
	// checks if the cursor position is on the top side of the first picture of the tile
	if (tile.angle == 0 || tile.angle == 180) {
		if (x > tile.tileParent.x && (y + (tileHeight / 2)) > tile.tileParent.y && x < (tile.tileParent.x + tileWidth) && (y + tileHeight / 2) < (tile.tileParent.y + (tileHeight / 2)))
			return true;
		else return false;
	}
	else if (tile.angle == 90 || tile.angle == 270) {
		if (x + tileWidth > tile.tileParent.x && y > tile.tileParent.y && x < tile.tileParent.x && y  < (tile.tileParent.y - (tileHeight / 2)))
			return true;
		else return false;
	}
	else if (tile.angle == -90 || tile.angle == -270) {
	if (x - tileWidth < tile.tileParent.x && y > tile.tileParent.y && x > tile.tileParent.x && y  < (tile.tileParent.y + (tileHeight / 2)))
		return true;
	else return false;
	}
}

bool Game::isClickedNextToTile(int x, int y, Tile tile) {
	// checks if the cursor position is on the right side og the first picture of the tile
	if (tile.angle == 0 || tile.angle == 180) {
		if ((x - tileWidth) > tile.tileParent.x && y > tile.tileParent.y && (x - tileWidth) < (tile.tileParent.x + tileWidth) && y < (tile.tileParent.y + (tileHeight / 2)))
			return true;
		else return false;
	}
	else if (tile.angle == 90 || tile.angle == 270) {
		if (x > tile.tileParent.x && y - tileHeight < tile.tileParent.y && x < (tile.tileParent.x + tileWidth) && y > tile.tileParent.y - tileHeight )
			return true;
		else return false;
	}
	else if (tile.angle == -90 || tile.angle == -270) {
		if (x < tile.tileParent.x && y > tile.tileParent.y + tileHeight / 2 && x > (tile.tileParent.x - tileWidth) && y + tileHeight < tile.tileParent.y )
			return true;
		else return false;
	}
	

}

bool Game::isClickedBeside2Tile(int x, int y, Tile tile) {
	// checks if the cursor position is on the left side of the second picture of the tile
	//cout << "x:" << x << " y:" << y << " tileX:" << tile.tileParent.x << " tileY:" << tile.tileParent.y << endl;
	if (tile.angle == 0 || tile.angle == 180) {
		if ((x + tileWidth) > tile.tileParent.x && y > tile.tileParent.y + tileHeight / 2 && (x + tileWidth) < (tile.tileParent.x + tileWidth) && y < (tile.tileParent.y + (tileHeight)))
			return true;
		else return false;
	}

	else if (tile.angle == 90 || tile.angle == 270) {
		if ((x - tileWidth) > tile.tileParent.x && y > tile.tileParent.y && (x - tileWidth) < (tile.tileParent.x + tileWidth) && y < (tile.tileParent.y + (tileHeight / 2)))
			return true;
		else return false;
	}
	else if (tile.angle == -90 || tile.angle == -270) {
		if ((x - tileWidth) > tile.tileParent.x && y > tile.tileParent.y && (x - tileWidth) < (tile.tileParent.x + tileWidth) && y < (tile.tileParent.y + (tileHeight / 2)))
			return true;
		else return false;
	}
}

bool Game::isClickedBelowTile(int x, int y, Tile tile) {
	// checks if the cursor position is on the botton side of the second picture of the tile
	if (x > (tile.tileParent.x) && (y - (tileHeight / 2)) > tile.tileParent.y + tileHeight / 2 && x < (tile.tileParent.x + tileWidth) && (y - (tileHeight / 2)) < (tile.tileParent.y + tileHeight))
		return true;
	else return false;
}

bool Game::isClickedNext2ToTile(int x, int y, Tile tile) {
	// checks if the cursor position is on the right side of the second picture of the tile
	if (tile.angle == 0 || tile.angle == 180) {
		if ((x - tileWidth) > tile.tileParent.x && y > tile.tileParent.y + tileHeight / 2 && (x - tileWidth) < (tile.tileParent.x + tileWidth) && y < (tile.tileParent.y + (tileHeight)))
			return true;
		else return false;
	}
	else if (tile.angle == 90 || tile.angle == 270) {
		if ((x - tileWidth) > tile.tileParent.x && y > tile.tileParent.y + tileHeight / 2 && x  < (tile.tileParent.x + tileWidth) && y < (tile.tileParent.y + (tileHeight)))
			return true;
		else return false;
	}
	else if (tile.angle == -90 || tile.angle == -270) {
		if ((x - tileWidth) > tile.tileParent.x && y > tile.tileParent.y - tileHeight / 2 && x  < tile.tileParent.x + tileHeight && y < (tile.tileParent.y - (tileHeight)))
			return true;
		else return false;
	}
}
void Game::reloadGame() {
	//when reloading the game we need to make tiles for both players and shuffle them
	//the same goes for the card of the table, and place it
	player1.deleteAllTilesAndPlayerData();
	player2.deleteAllTilesAndPlayerData();
	table.deleteAllTilesAndPlayerData();
	player1.createTiles(difficulty, ids, 45, 90);
	player1.shuffleTiles();
	player2.createTiles(difficulty, ids, 45, 90);
	player2.shuffleTiles();
	table.createFirstTileForTable(10, ids, 45, 90);
	player1.changeTurnStatus(1);
	player2.changeTurnStatus(0);
	xPosT = (destGrid.x + destGrid.w) / 2 - 45;
	yPosT = destGrid.y + (destGrid.h / 2) - 45;
	table.getTiles().at(0).changePosition(xPosT, yPosT);
}

void Game::moveTileToTableAbove(Player& player, int position, int side, float rotationOfTile, int changeX, int changeY) {
	//in here we get the index of the first picture of the first tile, it is always 0
	int firstTileId = 0;
	int lastTileId = 0;
	cout << "firstTileId " << firstTileId << endl;
	cout << "lastTileId " << lastTileId << endl;

	//we get the tile of the player
	Tile tile = player.getTiles().at(position);
	string begin = table.getTiles().at(0).getPictureid1();

	//in here we get the ids of its two pictures
	string tileFirstPicId = tile.getPictureid1();
	string tileSecondPicId = tile.getPictureid2();

	for (int i = 0; i < table.getCount(); i++) {
		table.getTiles().at(i).print();
	}

	//when we add above we add it to the beginning of the vector
	//if the id of the first tile of the vector of the table == the first id of the tile we want to add to the table
	if (begin == tileFirstPicId /*&& beginFreeSide == 1*/) {
		//table.getTiles().at(0).changeSideAvailability(2, 1);

		//we add it to the beggining of the vector and it becomes 0
		//cout << "getCount" << table.getCount() << endl;
		table.addTileToTableFirst(tile);

		//now we need to change the index of the tiles, because now the first element is the second
		firstTileId = 0;
		lastTileId = 1;

		xPosT = table.getTiles().at(lastTileId).tileParent.x;
		yPosT = table.getTiles().at(lastTileId).tileParent.y - 90;

		//in here we change the coordinats and angle of the new tile and draw it in the render fuction
		//we add 180 degrees, because if the ids don't match, we need to rotate it again to change the places of the pictures
		table.getTiles().at(0).changePosition(table.getTiles().at(1).tileParent.x + changeX, table.getTiles().at(1).tileParent.y + changeY, rotationOfTile + 180.0f);

		//deleting the tile of the player that is selected
		player.removeTile(position);
		for (int i = 0; i < table.getCount(); i++) {
			table.getTiles().at(i).print();
		}

	}//1

	//if the id of the first tile of the vector of the table == the second id of the tile we want to add to the table
	else if (begin == tileSecondPicId /*&& beginFreeSide == 1*/) {
		//table.getTiles().at(0).changeSideAvailability(2, 2);

		//we add it to the beggining of the vector and it becomes 0
		table.addTileToTableFirst(tile);
		cout << "getCount" << table.getCount() << endl;
		//we change again the indexes, the previous is already 1
		firstTileId = 0;
		lastTileId = 1;

		cout << table.getCount() << endl;

		//in here we change the coordinats and angle of the new tile and draw it in the render fuction
		xPosT = table.getTiles().at(lastTileId).tileParent.x;
		yPosT = table.getTiles().at(lastTileId).tileParent.y - 90;
		table.getTiles().at(0).changePosition(table.getTiles().at(1).tileParent.x + changeX, table.getTiles().at(1).tileParent.y + changeY, rotationOfTile);

		//we remove the tile that we already have on the table from the player
		player.removeTile(position);

		for (int i = 0; i < table.getCount(); i++) {
			table.getTiles().at(i).print();
		}

	}//3
}

void Game::moveTileToTableBelow(Player& player, int position, int side, float rotationOfTile, int changeX, int changeY) {
	//the tile of the player
	Tile tile = player.getTiles().at(position);

	//we set this indexes like this wo after we can change them to second to las element and last
	int firstTileId = (int)table.getCount() - 1;
	int lastTileId = (int)table.getCount() - 1;

	cout << "firstTileId " << firstTileId << endl;
	cout << "lastTileId " << lastTileId << endl;

	//in here we get the id of the second picture of the last tile
	string end = table.getTiles().at(lastTileId).getPictureid2();

	//and the two ids of the pictures of the tile we want to add
	string tileFirstPicId = tile.getPictureid1();
	string tileSecondPicId = tile.getPictureid2();

	for (int i = 0; i < table.getCount(); i++) {
		table.getTiles().at(i).print();
	}
	//if the id of the second picture of the last tile == first id of the chosen tile we want to add
	if (end == tileFirstPicId /*&& endFreeSide == 1*/) {
		//table.getTiles().at(last).changeSideAvailability(2, 1);

		//when we add below we add it to the back of the vector
		table.addTileToTable(tile);

		//now the last index is the recalculated becouse of the new tile
		lastTileId = (int)table.getCount() - 1;

		cout << "firstTileId " << firstTileId << endl;
		cout << "lastTileId " << lastTileId << endl;

		//we change the coordinates and angle of the tile that is now last
		table.getTiles().at(lastTileId).changePosition(table.getTiles().at(firstTileId).tileParent.x + changeX, table.getTiles().at(firstTileId).tileParent.y + changeY, rotationOfTile);

		//removing the selected tile of the player
		player.removeTile(position);
		cout << endl << endl;
		for (int i = 0; i < table.getCount(); i++) {
			table.getTiles().at(i).print();
		}

	}
	//if the id of the second picture of the last tile == second id of the chosen tile we want to add
	else if (end == tileSecondPicId/* && endFreeSide == 1*/) {
		//table.getTiles().at(last).changeSideAvailability(2, 1);

		//when we add below we add it to the back of the vector
		table.addTileToTable(tile);
		cout << endl << endl;

		cout << "firstTileId " << firstTileId << endl;
		cout << "lastTileId " << lastTileId << endl;
		///cout << "lastTileId " << lastTileId << endl;
		//cout << table.getCount() << endl;

		//recalvulating the new count
		lastTileId = (int)table.getCount() - 1;

		//in here we change the angle and positions according to the tile before it, and add 180 cause we need to change the places of the tiles inside
		table.getTiles().at(lastTileId).changePosition(table.getTiles().at(firstTileId).tileParent.x + changeX, table.getTiles().at(firstTileId).tileParent.y + changeY, rotationOfTile + 180.0f);

		//removing the selected tile from the player
		player.removeTile(position);
		for (int i = 0; i < table.getCount(); i++) {
			table.getTiles().at(i).print();
		}

	}//4

}
bool Game::isRunning() {
	return Game::running;
}

void Game::clean() {
	std::cout << "cleaning game\n";

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

Game::Game() {

	Game::window = NULL;
	Game::renderer = NULL;
	Game::running = true;
	Game::difficulty = 0;
	Game::xPosT = 0;
	Game::yPosT = 0;
	Game::tileWidth = 0;
	Game::tileHeight = 0;
	Game::destGrid = { 0, 0, 0, 0 };
	Game::destPlayer1 = { 0, 0, 0, 0 };
	Game::destPlayer2 = { 0, 0, 0, 0 };
	Game::selectedTile = 0;
	Game::destMenu = { 0, 0, 0, 0 };
	Game::destTheme = { 0, 0, 0, 0 };
	Game::destNewGame = { 0, 0, 0, 0 };
	Game::destGameMenu = { 0, 0, 0, 0 };
	Game::destPass = { 0, 0, 0, 0 };
	Game::menuClicked = 0;
	Game::passClicked = 0;
	Game::newGameClicked = 0;
	Game::themeClicked = 0;
	Game::visibility = { 0, 0, 0, 0, 0, 0 };
}

Game::~Game() {

}

bool Game::test() {
	/*Player player3;
	SDL_SetRenderDrawColor(renderer, 229, 255, 204, 255);
	SDL_RenderClear(renderer);
	int ww, wh;
	player3.createTiles(3, ids, 45, 90);
	player3.tilesOfPlayer.at(2).print();
	player3.tilesOfPlayer.at(2).changePosition(70, 80);
	cout << " x: " << player3.getTiles().at(2).tileParent.x << " y: " << player3.getTiles().at(2).tileParent.y << endl;
	TextureManager::Instance()->drawTile(1, player3.getTiles().at(2).getPictureid1(), player3.getTiles().at(2).getPictureid2(), 1 * (45 + 5) + 20, 0, player3.getTiles().at(2), 45, 90, renderer);
	SDL_GetWindowSize(window, &ww, &wh);
	SDL_RenderPresent(renderer);
	SDL_Delay(2000);*/
	return 0;
}