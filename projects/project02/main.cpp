#include "game.h"
#include <iostream>
#include <limits>

int main() 
{
    Game game;
    bool playAgain = true;

    while (playAgain) 
    {
        while (game.status() == Game::Status::ONGOING) 
        {
            std::cout << game;
            int row1, col1, row2, col2;
            while (true) 
            {
                std::cout << "Enter first gem coordinates (row col): ";
                if (std::cin >> row1 >> col1) 
                {
                    std::cout << "Enter second gem coordinates (row col): ";
                    if (std::cin >> row2 >> col2) 
                    {
                        if (row1 > 0 && row1 <= 8 && col1 > 0 && col1 <= 8 && row2 > 0 && row2 <= 8 && col2 > 0 && col2 <= 8) {
                            break;
                        }
                        else 
                        {
                            std::cout << "Coordinates out of range. Please enter valid coordinates.\n";
                        }
                    }
                    else 
                    {
                        std::cout << "Invalid input. Please enter numbers only.\n";
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
                else 
                {
                    std::cout << "Invalid input. Please enter numbers only.\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }
            game.play(row1 - 1, col1 - 1, row2 - 1, col2 - 1);
        }

        if (game.status() == Game::Status::PLAYER_1_WINS) 
        {
            std::cout << "AWESOME!!!! Player 1 is the Winner!\n";
        }
        else if (game.status() == Game::Status::PLAYER_2_WINS) 
        {
            std::cout << "AWESOME!!!! Player 2 is the Winner!\n";
        }
        else if (game.status() == Game::Status::DRAW) 
        {
            std::cout << "IT'S A DRAW!\n";
        }

        std::cout << "Play again? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice != 'y' && choice != 'Y') 
        {
            playAgain = false;
        }
        else 
        {
            game = Game();
        }
    }
    return 0;
}