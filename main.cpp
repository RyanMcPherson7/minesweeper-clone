#include <fstream>
#include "Board.h"

int main()
{
    //open configuration file and extract width, height, and numMines
    std::ifstream inFile("boards/config.cfg");
    int width, height, numMines;
    inFile >> width;
    inFile >> height;
    inFile >> numMines;
    inFile.close();

    //set the screen with proper dimensions
    sf::RenderWindow window(sf::VideoMode(width * 32, height * 32 + 88), "Minesweeper");

    //create and set up new board
    Board game(width, height, numMines);
    game.Reset();

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            //when user enters left or right click event
            if (event.type == sf::Event::MouseButtonPressed) {

                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (event.mouseButton.button == sf::Mouse::Left) {

                    // 1. if tile was clicked, reveal it and check if game is won or lost
                    for (unsigned int i = 0; i < height; i++)
                        for (unsigned int j = 0; j < width; j++)
                            if (game.GetGameBoard()[i][j].button.contains(mousePos.x, mousePos.y))
                                game.RevealTile(game.GetGameBoard()[i][j]);

                    // 2. if reset button is pressed, reset the game board
                    if (game.GetResetButton().contains(mousePos.x, mousePos.y))
                        game.Reset();

                    // 3. if debug button was pressed, reveal mine locations
                    if (game.GetDebugButton().contains(mousePos.x, mousePos.y))
                        game.UpdateShowMines();

                    // 4. if test 1 button is pressed, reset board to match "test_1"
                    if (game.GetTest1Button().contains(mousePos.x, mousePos.y))
                        game.ResetFromFile("boards/testboard1.brd");

                    // 5. if test 2 button is pressed, reset board to match "test_2"
                    if (game.GetTest2Button().contains(mousePos.x, mousePos.y))
                        game.ResetFromFile("boards/testboard2.brd");

                    // 6. if test 3 button is pressed, reset board to match "test_3"
                    if (game.GetTest3Button().contains(mousePos.x, mousePos.y))
                        game.ResetFromFile("boards/testboard3.brd");
                }

                if (event.mouseButton.button == sf::Mouse::Right) {

                    // 1. if user flags a tile
                    for (unsigned int i = 0; i < height; i++)
                        for (unsigned int j = 0; j < width; j++)
                            if (game.GetGameBoard()[i][j].button.contains(mousePos.x, mousePos.y))
                                game.UpdateFlagState(game.GetGameBoard()[i][j]);
                }
            }
        }

        //draw game to the screen
        window.clear();

        game.DrawBoard(window);

        window.display();
    }

    //clear all stored textures
    TextureManager::Clear();

    return 0;
}