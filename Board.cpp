#include "Board.h"
#include <ctime>
#include <fstream>

Tile::Tile(const string& _property, const string& _cover, const string& _backdrop, const string& _flag) {

    //set sprites based on given textures
    property.setTexture(TextureManager::GetTexture(_property));
    cover.setTexture(TextureManager::GetTexture(_cover));
    backdrop.setTexture(TextureManager::GetTexture(_backdrop));
    flag.setTexture(TextureManager::GetTexture(_flag));

    isFlagged = false;
    isMine = false;
    isRevealed = false;
    isBlank = true;
}


Board::Board(int _width, int _height, int _numMines) {

    width = _width;
    height = _height;
    numMines = _numMines;
    configNumMines = _numMines;
    flagCounter = _numMines;
    revealedCount = 0;

    DEBUG_showMines = false;
    isLocked = false;

    //assign texture images to Sprites
    resetSprite.setTexture(TextureManager::GetTexture("face_happy"));
    debugSprite.setTexture(TextureManager::GetTexture("debug"));
    test1Sprite.setTexture(TextureManager::GetTexture("test_1"));
    test2Sprite.setTexture(TextureManager::GetTexture("test_2"));
    test3Sprite.setTexture(TextureManager::GetTexture("test_3"));
    flagScoreSpriteLeft.setTexture((TextureManager::GetTexture("digits")));
    flagScoreSpriteMid.setTexture((TextureManager::GetTexture("digits")));
    flagScoreSpriteRight.setTexture((TextureManager::GetTexture("digits")));

    //set the board positions for Sprites
    resetSprite.setPosition(((float)width - 2.f) * 16.f, (float)height * 32.f);
    debugSprite.setPosition((float)width * 16.f + 64.f, (float)height * 32.f);
    test1Sprite.setPosition((float)width * 16.f + 128.f, (float)height * 32.f);
    test2Sprite.setPosition((float)width * 16.f + 192.f, (float)height * 32.f);
    test3Sprite.setPosition((float)width * 16.f + 256.f, (float)height * 32.f);
    flagScoreSpriteLeft.setPosition(0, (float)height * 32);
    flagScoreSpriteMid.setPosition(21, (float)height * 32);
    flagScoreSpriteRight.setPosition(42, (float)height * 32);

    //set each score Sprite to display number 0
    flagScoreSpriteLeft.setTextureRect(sf::IntRect(0, 0, 21, 32));
    flagScoreSpriteMid.setTextureRect(sf::IntRect(0, 0, 21, 32));
    flagScoreSpriteRight.setTextureRect(sf::IntRect(0, 0, 21, 32));

    //set the float recs to hold the board positions of the buttons
    resetButton = resetSprite.getGlobalBounds();
    debugButton = debugSprite.getGlobalBounds();
    test1Button = test1Sprite.getGlobalBounds();
    test2Button = test2Sprite.getGlobalBounds();
    test3Button = test3Sprite.getGlobalBounds();
}


vector<vector<Tile>>& Board::GetGameBoard() { return gameBoard; }


//clears the old board and creates a new one
void Board::Reset() {

    //resets the board's original data and builds new tiles
    CleanAndRebuild();

    numMines = configNumMines;
    flagCounter = numMines;

    //seed RNG
    srand(time(nullptr));

    //placing mines in random tiles
    unsigned int count = 0;

    while (count < numMines) {

        unsigned int randHeight = rand() % height;
        unsigned int randWidth = rand() % width;

        if (!gameBoard[randHeight][randWidth].isMine) { //if the tile is not a mine already...

            gameBoard[randHeight][randWidth].property.setTexture(TextureManager::GetTexture("mine"));
            gameBoard[randHeight][randWidth].isMine = true;
            gameBoard[randHeight][randWidth].isBlank = false;
            count++;
        }
    }

    //update's each individual tile's neighboring tiles and sets their property (number or blank)
    UpdateNeighborsAndProperties();
}


//given a file of 1's and 0's create a board where all 1's represent a mine location
void Board::ResetFromFile(const string &fileName) {

    //resets the board's original data and builds new tiles
    CleanAndRebuild();

    //laying out mines according to file
    std::fstream inFile(fileName);
    char currentCell;
    int mineCount = 0;

    for (unsigned int i = 0; i < height; i++) {

        for (unsigned int j = 0; j < width; j++) {

            inFile.get(currentCell);

            if (currentCell == '1') {  //if the cell is 1, then it's a mine

                gameBoard[i][j].property.setTexture(TextureManager::GetTexture("mine"));
                gameBoard[i][j].isMine = true;
                gameBoard[i][j].isBlank = false;

                mineCount++;
            }
        }
        //collect the new line character
        inFile.get();
    }

    inFile.close();

    //update number of mines
    numMines = mineCount;
    flagCounter = numMines;

    //update's each individual tile's neighboring tiles and sets their property (number or blank)
    UpdateNeighborsAndProperties();
}


void Board::DrawBoard(sf::RenderWindow &window) {

    //drawing tiles

    for (unsigned int i = 0; i < height; i++) {

        for (unsigned int j = 0; j < width; j++) {

            Tile& currentTile = gameBoard[i][j];


            //update the flag's position and draw it if bool is true

            window.draw(currentTile.backdrop);
            window.draw(currentTile.property);

            if (!currentTile.isRevealed)  //if has not been revealed, draw the cover
                window.draw(currentTile.cover);

            if (currentTile.isFlagged)  //if it is flagged, draw the flag
                window.draw(currentTile.flag);

            if (DEBUG_showMines)  //if DEBUG to show mines is on, display the tiles with mines on top
                if (currentTile.isMine)
                    window.draw(currentTile.property);
        }
    }

    //drawing board

    //draw white background for scoreboard
    sf::RectangleShape scoreBackground(sf::Vector2f(width * 32, 88));
    scoreBackground.setPosition(0, (float)height * 32);
    window.draw(scoreBackground);

    //draw buttons
    window.draw(resetSprite);
    window.draw(debugSprite);
    window.draw(test1Sprite);
    window.draw(test2Sprite);
    window.draw(test3Sprite);

    //update the flag score board

    if (flagCounter < 0) {  //if negative

        flagScoreSpriteLeft.setTextureRect(sf::IntRect(210, 0, 21, 32));
        flagScoreSpriteMid.setTextureRect(sf::IntRect(flagCounter / 10 * 21 * - 1, 0, 21, 32));
        flagScoreSpriteRight.setTextureRect(sf::IntRect(flagCounter % 10 * 21 * - 1, 0, 21, 32));
    }
    else {  //if positive

        flagScoreSpriteLeft.setTextureRect(sf::IntRect(0, 0, 21, 32));
        flagScoreSpriteMid.setTextureRect(sf::IntRect(flagCounter / 10 * 21, 0, 21, 32));
        flagScoreSpriteRight.setTextureRect(sf::IntRect(flagCounter % 10 * 21, 0, 21, 32));
    }

    //draw flag score board
    window.draw(flagScoreSpriteLeft);
    window.draw(flagScoreSpriteMid);
    window.draw(flagScoreSpriteRight);
}


//updates the given cell's flag status and updates flag count accordingly
void Board::UpdateFlagState(Tile& cell) {

    if (cell.isRevealed || isLocked)  //if the tile is already revealed, don't flag it
        return;

    cell.isFlagged = !cell.isFlagged;

    if (cell.isFlagged)
        flagCounter--;
    else
        flagCounter++;
}


//run if game is won (all non mine tiles revealed)
void Board::Win() {

    resetSprite.setTexture(TextureManager::GetTexture("face_win"));

    //flag all remaining tiles (the mines)
    for (unsigned int i = 0; i < height; i++)
        for (unsigned int j = 0; j < width; j++)
            if (!gameBoard[i][j].isRevealed)
                UpdateFlagState(gameBoard[i][j]);

    isLocked = true;
}


//run if the game is lost (mine is revealed)
void Board::Lose() {

    resetSprite.setTexture(TextureManager::GetTexture("face_lose"));

    isLocked = true;
}


//reveals tile and checks if game is won, lost, or continues
void Board::RevealTile(Tile& cell) {

    if (cell.isFlagged || cell.isRevealed || isLocked)  //if the tile is flagged, don't reveal it
        return;

    cell.isRevealed = true;
    revealedCount++;

    if (cell.isMine)  //if a revealed tile is a mine, user loses
        Lose();

    if (revealedCount == height * width - numMines) //if the right amount of tiles is revealed, user wins
        Win();

    if (cell.isBlank)
        for (unsigned int i = 0; i < cell.neighbors.size(); i++)
            if (!cell.neighbors[i]->isRevealed)  //if the tile is not already revealed, reveal it
                RevealTile(*cell.neighbors[i]);
}


//helper function to reset and rebuild game board
void Board::CleanAndRebuild() {

    //reset values
    revealedCount = 0;
    resetSprite.setTexture(TextureManager::GetTexture("face_happy"));
    isLocked = false;

    //reset and rebuild tiles
    gameBoard.clear();

    //populate board with tiles
    for (unsigned int i = 0; i < height; i++) {

        vector<Tile> temp;  //store each individual line of tiles

        for (unsigned int j = 0; j < width; j++) {

            Tile currentTile("tile_revealed", "tile_hidden", "tile_revealed", "flag");

            //set the board positions of the tiles
            currentTile.backdrop.setPosition((float)j * 32.f, (float)i * 32.f);
            currentTile.property.setPosition((float)j * 32.f, (float)i * 32.f);
            currentTile.cover.setPosition((float)j * 32.f, (float)i * 32.f);
            currentTile.flag.setPosition((float)j * 32.f, (float)i * 32.f);

            //update the float rect to represent the tile's "clickable" location
            currentTile.button = currentTile.backdrop.getGlobalBounds();

            temp.push_back(currentTile);

        }
        gameBoard.push_back(temp);
    }
}


//helper function to update each tile's neighbors and its property (number, blank)
void Board::UpdateNeighborsAndProperties() {

    //fill in tile's neighbors
    //i will serve as the y cord, and j as the x cord
    for (int i = 0; i < height; i++) {

        for (int j = 0; j < width; j++) {

            Tile& current = gameBoard[i][j];

            //top row
            for (int x = j - 1; x < j + 2; x++)
                if (x >= 0 && x < width && i - 1 >= 0 && i - 1 < height)  //check to see if tile is in bounds,
                    current.neighbors.push_back(&gameBoard[i - 1][x]);    //if so add it's address to the list

            //middle row
            for (int x = j - 1; x < j + 2; x += 2)
                if (x >= 0 && x < width && i >= 0 && i < height)
                    current.neighbors.push_back(&gameBoard[i][x]);

            //bottom row
            for (int x = j - 1; x < j + 2; x++)
                if (x >= 0 && x < width && i + 1 >= 0 && i + 1 < height)
                    current.neighbors.push_back(&gameBoard[i + 1][x]);
        }
    }

    //check the number of neighboring mines and update the Sprites accordingly, skipping over any tiles that are mines
    for (unsigned int i = 0; i < height; i++) {

        for (unsigned int j = 0; j < width; j++) {

            Tile& current = gameBoard[i][j];
            unsigned int neighborMineCount = 0;

            //if the current tile is a mine, don't update the tile
            if (current.isMine)
                continue;

            //calculate number of neighboring mines
            for (unsigned int k = 0; k < current.neighbors.size(); k++)
                if (current.neighbors[k]->isMine) //if neighbor is mine, increase count by 1
                    neighborMineCount++;

            //update Sprite based on count
            if (neighborMineCount > 0) {
                current.property.setTexture(TextureManager::GetTexture("number_" + std::to_string(neighborMineCount)));
                current.isBlank = false;
            }
        }
    }
}


void Board::UpdateShowMines() { DEBUG_showMines = !DEBUG_showMines; }

sf::FloatRect Board::GetResetButton() { return resetButton; }

sf::FloatRect Board::GetDebugButton() { return debugButton; }

sf::FloatRect Board::GetTest1Button() { return test1Button; }

sf::FloatRect Board::GetTest2Button() { return test2Button; }

sf::FloatRect Board::GetTest3Button() { return test3Button; }