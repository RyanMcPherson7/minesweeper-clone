#pragma once
#include "TextureManager.h"
#include <vector>

using std::vector;

struct Tile {

    sf::Sprite property;
    sf::Sprite cover;
    sf::Sprite backdrop;
    sf::Sprite flag;
    sf::FloatRect button;
    bool isFlagged;
    bool isMine;
    bool isRevealed;
    bool isBlank;
    vector<Tile*> neighbors;

    Tile(const string& _property, const string& _cover, const string& _backdrop, const string& _flag);
};

class Board {

    sf::Sprite resetSprite;
    sf::Sprite debugSprite;
    sf::Sprite test1Sprite;
    sf::Sprite test2Sprite;
    sf::Sprite test3Sprite;
    sf::Sprite flagScoreSpriteLeft;
    sf::Sprite flagScoreSpriteMid;
    sf::Sprite flagScoreSpriteRight;

    //each Sprite contains its own rect Object, found this out late in the project and
    //don't feel like changing this
    sf::FloatRect resetButton;
    sf::FloatRect debugButton;
    sf::FloatRect test1Button;
    sf::FloatRect test2Button;
    sf::FloatRect test3Button;

    vector<vector<Tile>> gameBoard;
    int flagCounter;
    int revealedCount;
    int width;
    int height;
    int numMines;
    int configNumMines;
    bool DEBUG_showMines;
    bool isLocked;
    void CleanAndRebuild();
    void UpdateNeighborsAndProperties();
public:
    vector<vector<Tile>>& GetGameBoard();
    Board(int _width, int _height, int _numMines);
    void DrawBoard(sf::RenderWindow& window);
    void Reset();
    void ResetFromFile(const string& fileName);
    void UpdateFlagState(Tile& cell);
    void UpdateShowMines();
    void RevealTile(Tile& cell);
    void Win();
    void Lose();
    sf::FloatRect GetResetButton();
    sf::FloatRect GetDebugButton();
    sf::FloatRect GetTest1Button();
    sf::FloatRect GetTest2Button();
    sf::FloatRect GetTest3Button();
};