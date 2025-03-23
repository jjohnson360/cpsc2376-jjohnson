#include "game.h"
#include <random>
#include <limits>
#include <iomanip>

// Constructor: Initializes the game board and player scores
Game::Game(int rows, int cols) : rows(rows), cols(cols), currentPlayer(1), player1Score(0), player2Score(0), combo(1) 
{
    makeBoard();
}

// Plays a move: Swaps gems and updates game state
void Game::play(int row1, int col1, int row2, int col2) 
{
    if (isValidMove(row1, col1, row2, col2)) 
    {
        std::swap(board[row1][col1], board[row2][col2]); // Swap the gems
        if (matchGems(row1, col1) || matchGems(row2, col2)) // Check for matches
        { 
            removeMatches(currentPlayer, combo); // Remove matches and update scores
            dropGems(); // Drop gems
            fillBoard(); // Fill empty spaces
            if (!hasValidMoves()) // Check for valid moves
            { 
                makeBoard(); // Reshuffle if no valid moves
                combo = 1;
            }
            else 
            {
                combo++; // Increment combo if there are still valid moves
            }
        }
        else 
        {
            std::swap(board[row1][col1], board[row2][col2]); // Swap back if no matches
            combo = 1; // Reset combo
        }
        // Switch players
        if (currentPlayer == 1) 
        {
            currentPlayer = 2;
        }
        else 
        {
            currentPlayer = 1;
        }
    }
    else 
    {
        combo = 1; // Reset combo for invalid moves
    }
}

// Returns the current game status
Game::Status Game::status() const 
{
    const int winningScore = 5000;
    if (player1Score >= winningScore) return Status::PLAYER_1_WINS;
    if (player2Score >= winningScore) return Status::PLAYER_2_WINS;
    return Status::ONGOING;
}

// Displays the game board to the console
void Game::display() const 
{
    std::cout << *this;
}

// Overloads the << operator to print the game board
std::ostream& operator<<(std::ostream& os, const Game& game) 
{
    os << "     ";
    for (int j = 0; j < game.cols; ++j) 
    {
        os << std::setw(5) << j + 1 << " ";
    }
    os << std::endl;

    for (int i = 0; i < game.rows; ++i) 
    {
        os << std::setw(3) << i + 1 << "  ";
        for (const auto& gem : game.board[i]) 
        {
            if (gem == Game::RED_GEM) 
            {
                os << "\033[31m" << std::setw(5) << "R" << "\033[0m" << " ";
            }
            else if (gem == Game::GREEN_GEM) 
            {
                os << "\033[32m" << std::setw(5) << "G" << "\033[0m" << " ";
            }
            else if (gem == Game::YELLOW_GEM) 
            {
                os << "\033[33m" << std::setw(5) << "Y" << "\033[0m" << " ";
            }
            else if (gem == Game::BLUE_GEM) 
            {
                os << "\033[34m" << std::setw(5) << "B" << "\033[0m" << " ";
            }
            else if (gem == Game::MAGENTA_GEM) 
            {
                os << "\033[35m" << std::setw(5) << "P" << "\033[0m" << " ";
            }
            else if (gem == Game::EMPTY) 
            {
                os << std::setw(5) << "." << " ";
            }
        }
        os << std::endl;
    }
    os << "Player 1 Score: " << game.player1Score << std::endl;
    os << "Player 2 Score: " << game.player2Score << std::endl;
    return os;
}

// Initializes the game board with random gems
void Game::makeBoard() 
{
    board.assign(rows, std::vector<GemType>(cols));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 5);

    for (int i = 0; i < rows; ++i) 
    {
        for (int j = 0; j < cols; ++j) 
        {
            board[i][j] = static_cast<GemType>(distrib(gen));
        }
    }
    // Ensure the board has valid moves
    while (!hasValidMoves()) 
    {
        for (int i = 0; i < rows; ++i) 
        {
            for (int j = 0; j < cols; ++j) 
            {
                board[i][j] = static_cast<GemType>(distrib(gen));
            }
        }
    }
}

// Checks if a move is valid
bool Game::isValidMove(int row1, int col1, int row2, int col2) 
{
    return (row1 == row2 && std::abs(col1 - col2) == 1) || (col1 == col2 && std::abs(row1 - row2) == 1);
}

// Checks if a gem has a match
bool Game::matchGems(int row, int col) 
{
    if (board[row][col] == GemType::EMPTY) return false;
    GemType gem = board[row][col];

    int matchCount = 1, left = col - 1, right = col + 1;
    while (left >= 0 && board[row][left] == gem) { matchCount++; left--; }
    while (right < cols && board[row][right] == gem) { matchCount++; right++; }
    if (matchCount >= 3) return true;

    matchCount = 1, left = row - 1, right = row + 1;
    while (left >= 0 && board[left][col] == gem) { matchCount++; left--; }
    while (right < rows && board[right][col] == gem) { matchCount++; right++; }
    return matchCount >= 3;
}

// Removes matched gems and updates score
void Game::removeMatches(int player, int combo) 
{
    for (int i = 0; i < rows; ++i) 
    {
        for (int j = 0; j < cols; ++j) 
        {
            if (matchGems(i, j)) 
            {
                if (player == 1) 
                {
                    player1Score += calculatePoints(board[i][j]) * combo;
                }
                else 
                {
                    player2Score += calculatePoints(board[i][j]) * combo;
                }
                board[i][j] = GemType::EMPTY;
            }
        }
    }
}

// Drops gems to fill empty spaces
void Game::dropGems() 
{
    for (int col = 0; col < cols; ++col) 
    {
        for (int row = rows - 1; row >= 0; --row) 
        {
            if (board[row][col] == GemType::EMPTY) 
            {
                for (int above = row - 1; above >= 0; --above) 
                {
                    if (board[above][col] != GemType::EMPTY) 
                    {
                        board[row][col] = board[above][col];
                        board[above][col] = GemType::EMPTY;
                        break;
                    }
                }
            }
        }
    }
}

// Fills empty spaces with new random gems
void Game::fillBoard() 
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 5);
    for (int i = 0; i < rows; ++i) 
    {
        for (int j = 0; j < cols; ++j) 
        {
            if (board[i][j] == GemType::EMPTY) 
            {
                board[i][j] = static_cast<GemType>(distrib(gen));
            }
        }
    }
}

// Calculates points based on gem type
int Game::calculatePoints(GemType gem) 
{
            if (gem == Game::RED_GEM) 
            {
                return 300;
            }
            else if (gem == Game::GREEN_GEM) 
            {
                return 250;
            }
            else if (gem == Game::YELLOW_GEM) 
            {
                return 200;
            }
            else if (gem == Game::BLUE_GEM) 
            {
                return 150;
            }
            else if (gem == Game::MAGENTA_GEM) 
            {
                return 100;
            }
            else 
            {
                return 0;
            }
        }

        // Checks if there are any valid moves left
        bool Game::hasValidMoves() 
        {
            for (int i = 0; i < rows; ++i) 
            {
                for (int j = 0; j < cols; ++j) 
                {
                    if (board[i][j] != GemType::EMPTY) 
                    {
                        if (j > 0 && (board[i][j] == board[i][j - 1] || checkPotentialMatch(i, j, i, j - 1))) return true;
                        if (j < cols - 1 && (board[i][j] == board[i][j + 1] || checkPotentialMatch(i, j, i, j + 1))) return true;
                        if (i > 0 && (board[i][j] == board[i - 1][j] || checkPotentialMatch(i, j, i - 1, j))) return true;
                        if (i < rows - 1 && (board[i][j] == board[i + 1][j] || checkPotentialMatch(i, j, i + 1, j))) return true;
                    }
                }
            }
            return false;
        }

        // Checks for potential matches
        bool Game::checkPotentialMatch(int row1, int col1, int row2, int col2) 
        {
            std::swap(board[row1][col1], board[row2][col2]);
            bool match = matchGems(row1, col1) || matchGems(row2, col2);
            std::swap(board[row1][col1], board[row2][col2]);
            return match;
        }