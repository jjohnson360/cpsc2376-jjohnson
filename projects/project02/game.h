#ifndef GAME_H
#define GAME_H

#include <vector>
#include <iostream>

class Game 
{
public:
    // Enumeration for game status
    enum Status { ONGOING, PLAYER_1_WINS, PLAYER_2_WINS, DRAW };
    // Enumeration for gem types
    enum GemType { EMPTY, RED_GEM, GREEN_GEM, YELLOW_GEM, BLUE_GEM, MAGENTA_GEM };

    // Constructor: Initializes the game board and player scores
    Game(int rows = 8, int cols = 8);

    // Plays a move: Swaps gems and updates game state
    void play(int row1, int col1, int row2, int col2);

    // Returns the current game status
    Status status() const;

    // Displays the game board to the console
    void display() const;

    // Overloads the << operator to print the game board
    friend std::ostream& operator<<(std::ostream& os, const Game& game);

private:
    std::vector<std::vector<GemType>> board; // 2D vector for the game board
    int currentPlayer; // Tracks the current player's turn
    int player1Score; // Player 1's score
    int player2Score; // Player 2's score
    int combo; // Tracks the current combo multiplier
    int rows; // Number of rows in the board
    int cols; // Number of columns in the board

    // Helper functions
    void makeBoard(); // Initializes the game board with random gems
    bool isValidMove(int row1, int col1, int row2, int col2); // Checks if a move is valid
    bool matchGems(int row, int col); // Checks if a gem has a match
    void removeMatches(int player, int combo); // Removes matched gems and updates score
    void dropGems(); // Drops gems to fill empty spaces
    void fillBoard(); // Fills empty spaces with new random gems
    int calculatePoints(GemType gem); // Calculates points based on gem type
    bool hasValidMoves(); // Checks if there are any valid moves left
    bool checkPotentialMatch(int row1, int col1, int row2, int col2); // Checks for potential matches
};

#endif // GAME_H