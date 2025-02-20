#include <iostream>
#include <vector>
#include <random>
#include <limits>
#include <iomanip>

// Color codes for terminal output
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"

// Enum for different game states
enum class GameState { ONGOING, PLAYER_1_WINS, PLAYER_2_WINS, DRAW };
// Enum for different gem types
enum class GemType { EMPTY, RED_GEM, GREEN_GEM, YELLOW_GEM, BLUE_GEM, MAGENTA_GEM };

// Structure to represent a player
struct Player 
{
    int score = 0;
};

// Global variables
std::vector<std::vector<GemType>> board; // 2D vector to represent the game board
Player player1, player2; // Player objects
int currentPlayer = 1; // Variable to keep track of the current player

// Function Declarations
void printRules();
void makeBoard(int rows, int cols);
void printBoard();
GameState gameStatus();
int play(int combo);
void swapGems(int row1, int col1, int row2, int col2, bool printMessage = true);
bool matchGems(int row, int col);
void removeMatches(Player& player, int combo);
void dropGems();
void fillBoard();
std::pair<int, int> getUserInput(const std::string& prompt);
bool isValidMove(int row1, int col1, int row2, int col2);
int calculatePoints(GemType gem);
bool hasValidMoves();
bool checkPotentialMatch(int row1, int col1, int row2, int col2);

int main() 
{
    printRules(); // Display the game rules

	bool playAgain = true; // Variable to control the game loop
    while (playAgain) 
	{ 
        makeBoard(8, 8); // Initialize the game board with 8 rows and 8 columns
        player1.score = 0; // Reset player scores
        player2.score = 0;
        currentPlayer = 1; // Start with player 1
        int combo = 1; // Initialize combo counter

        while (gameStatus() == GameState::ONGOING) // Play until the game is over
        { 
            std::cout << "Game in Progress\n"; // Display game status
            combo = play(combo); // Play a turn and update combo
        }

        // Display game end message based on game status
        if (gameStatus() == GameState::PLAYER_1_WINS) 
        {
            std::cout << "AWESOME!!!! Player 1 is the Winner!\n";
        }
        else if (gameStatus() == GameState::PLAYER_2_WINS) 
        {
            std::cout << "AWESOME!!!! Player 2 is the Winner!\n";
        }
        else if (gameStatus() == GameState::DRAW) 
        {
            std::cout << "IT'S A DRAW!\n";
        }

        // Ask the player if they want to play again
        std::cout << "Play again? (y/n): ";
        char choice;
        std::cin >> choice;
        if (choice != 'y' && choice != 'Y') 
        {
            playAgain = false; // Exit the game loop if the player doesn't want to play again
        }
    }

    return 0;
}

void printRules() 
{
    std::cout << "Welcome to Bejeweled!\n";
    std::cout << "Players swap adjacent gems to create matches of three or more.\n";
    std::cout << "Matches score points. The first player to 5000 wins.\n";
    std::cout << "Points per match:\n";
    std::cout << "RED = 300 points each\n";
    std::cout << "GREEN = 250 points each\n";
    std::cout << "YELLOW = 200 points each\n";
    std::cout << "BLUE = 150 points each\n";
    std::cout << "MAGENTA = 100 points each\n";
}

void makeBoard(int rows, int cols) 
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

void printBoard() {
    std::cout << "     "; 
    for (int j = 0; j < static_cast<int>(board.size()); ++j) 
    { 
        std::cout << std::setw(5) << j + 1 << " "; 
    }
    std::cout << std::endl;

    for (int i = 0; i < static_cast<int>(board.size()); ++i) 
    {
        std::cout << std::setw(3) << i + 1 << "  "; // Print row numbers
        for (const auto& gem : board[i]) // Iterate through each gem in the row
        { 
            if (gem == GemType::RED_GEM) 
            {
                std::cout << RED << std::setw(5) << "R" << RESET << " ";
            }
            else if (gem == GemType::BLUE_GEM) 
            {
                std::cout << BLUE << std::setw(5) << "B" << RESET << " ";
            }
            else if (gem == GemType::GREEN_GEM) 
            {
                std::cout << GREEN << std::setw(5) << "G" << RESET << " ";
            }
            else if (gem == GemType::YELLOW_GEM) 
            {
                std::cout << YELLOW << std::setw(5) << "Y" << RESET << " ";
            }
            else if (gem == GemType::MAGENTA_GEM) 
            {
                std::cout << MAGENTA << std::setw(5) << "P" << RESET << " ";
            }
            else if (gem == GemType::EMPTY) 
            {
                std::cout << std::setw(5) << "." << " ";
            }
        }
        std::cout << std::endl;
    }
}

GameState gameStatus() {
    const int winningScore = 5000; // Winning score is 5000

    if (player1.score >= winningScore) return GameState::PLAYER_1_WINS;
    if (player2.score >= winningScore) return GameState::PLAYER_2_WINS;

    return GameState::ONGOING;
}

int play(int combo) 
{
    printBoard();
    if (currentPlayer == 1) 
    {
        std::cout << "Player 1's turn.\n";
    }
    else 
    {
        std::cout << "Player 2's turn.\n";
    }

    std::pair<int, int> gem1 = getUserInput("Enter first gem coordinates (row col): ");
    std::pair<int, int> gem2 = getUserInput("Enter second gem coordinates (row col): ");

    std::cout << "Swapping gems at (" << gem1.first << ", " << gem1.second << ") and (" << gem2.first << ", " << gem2.second << ")\n";

    if (isValidMove(gem1.first - 1, gem1.second - 1, gem2.first - 1, gem2.second - 1)) 
    {
        swapGems(gem1.first - 1, gem1.second - 1, gem2.first - 1, gem2.second - 1); 
        printBoard(); 

        if (matchGems(gem1.first - 1, gem1.second - 1) || matchGems(gem2.first - 1, gem2.second - 1)) 
        {
            if (currentPlayer == 1) 
            {
                removeMatches(player1, combo);
            }
            else 
            {
                removeMatches(player2, combo);
            }
            dropGems();
            fillBoard();
            if (!hasValidMoves()) 
            {
                std::cout << "No more valid moves. Reshuffling the board.\n";
                makeBoard(8, 8);
                printBoard();
                combo = 1;
            }
            else 
            {
                combo++;
            }

        }
        else 
        {
            swapGems(gem1.first - 1, gem1.second - 1, gem2.first - 1, gem2.second - 1);
            std::cout << "No match formed! Try again.\n";
            combo = 1;
        }
    }
    else 
    {
        std::cout << "Invalid move. Try again.\n";
        combo = 1;
    }

    if (currentPlayer == 1) 
    {
        std::cout << "Player 1's current score: " << player1.score << std::endl;
        currentPlayer = 2;
    }
    else 
    {
        std::cout << "Player 2's current score: " << player2.score << std::endl;
        currentPlayer = 1;
    }
    return combo;
}

void swapGems(int row1, int col1, int row2, int col2, bool printMessage) { 
    if (printMessage) 
    {
        std::cout << "Swapping (" << row1 + 1 << ", " << col1 + 1 << ") with (" << row2 + 1 << ", " << col2 + 1 << ")\n";
    }
    std::swap(board[row1][col1], board[row2][col2]);
}

bool matchGems(int row, int col) 
{
    if (board[row][col] == GemType::EMPTY) return false;
    GemType gem = board[row][col];

    int matchCount = 1, left = col - 1, right = col + 1;
    while (left >= 0 && board[row][left] == gem) { matchCount++; left--; }
    while (right < static_cast<int>(board.size()) && board[row][right] == gem) { matchCount++; right++; }
    if (matchCount >= 3) return true;

    matchCount = 1, left = row - 1, right = row + 1;
    while (left >= 0 && board[left][col] == gem) { matchCount++; left--; }
    while (right < static_cast<int>(board.size()) && board[right][col] == gem) { matchCount++; right++; }
    return matchCount >= 3;
}

void removeMatches(Player& player, int combo) 
{
    for (int i = 0; i < static_cast<int>(board.size()); ++i) 
    {
        for (int j = 0; j < static_cast<int>(board.size()); ++j) 
        {
            if (matchGems(i, j)) 
            {
                player.score += calculatePoints(board[i][j]) * combo;
                board[i][j] = GemType::EMPTY;
            }
        }
    }
}

void dropGems() {
    for (int col = 0; col < static_cast<int>(board.size()); ++col) 
    {
        for (int row = static_cast<int>(board.size()) - 1; row >= 0; --row) 
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

void fillBoard() 
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 5);
    for (int i = 0; i < static_cast<int>(board.size()); ++i) 
    {
        for (int j = 0; j < static_cast<int>(board.size()); ++j) 
        {
            if (board[i][j] == GemType::EMPTY) 
            {
                board[i][j] = static_cast<GemType>(distrib(gen));
            }
        }
    }
}

std::pair<int, int> getUserInput(const std::string& prompt) 
{
    int row, col;
    while (true) 
    {
        std::cout << prompt;
        if (std::cin >> row >> col) 
        {
            if (row > 0 && row <= board.size() && col > 0 && col <= board.size()) 
            {
                return { row, col };
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
}


bool isValidMove(int row1, int col1, int row2, int col2) 
{
    return (row1 == row2 && std::abs(col1 - col2) == 1) || (col1 == col2 && std::abs(row1 - row2) == 1);
}

int calculatePoints(GemType gem) 
{
    if (gem == GemType::RED_GEM) 
    {
        return 300;
    }
    else if (gem == GemType::GREEN_GEM) 
    {
        return 250;
    }
    else if (gem == GemType::YELLOW_GEM) 
    {
        return 200;
    }
    else if (gem == GemType::BLUE_GEM) 
    {
        return 150;
    }
    else if (gem == GemType::MAGENTA_GEM) 
    {
        return 100;
    }
    else 
    {
        return 0;
    }
}

bool hasValidMoves() {
    for (int i = 0; i < static_cast<int>(board.size()); ++i) 
    {
        for (int j = 0; j < static_cast<int>(board.size()); ++j) 
        {
            if (board[i][j] != GemType::EMPTY) 
            {
                // Check adjacent gems for potential matches
                if (j > 0 && board[i][j - 1] != GemType::EMPTY && (board[i][j] == board[i][j - 1] || checkPotentialMatch(i, j, i, j - 1))) return true;
                if (j < static_cast<int>(board.size()) - 1 && board[i][j + 1] != GemType::EMPTY && (board[i][j] == board[i][j + 1] || checkPotentialMatch(i, j, i, j + 1))) return true;
                if (i > 0 && board[i - 1][j] != GemType::EMPTY && (board[i][j] == board[i - 1][j] || checkPotentialMatch(i, j, i - 1, j))) return true;
                if (i < static_cast<int>(board.size()) - 1 && board[i + 1][j] != GemType::EMPTY && (board[i][j] == board[i + 1][j] || checkPotentialMatch(i, j, i + 1, j))) return true;
            }
        }
    }
    return false;
}

bool checkPotentialMatch(int row1, int col1, int row2, int col2) 
{
    swapGems(row1, col1, row2, col2, false); 
    bool match = matchGems(row1, col1) || matchGems(row2, col2);
    swapGems(row1, col1, row2, col2, false); 
    return match;
}