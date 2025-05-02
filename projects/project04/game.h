// game.h
#pragma once
#include <vector>
#include <SDL2/SDL.h>

class Game {
public:
    enum GemType { EMPTY, RED_GEM, GREEN_GEM, YELLOW_GEM, BLUE_GEM, MAGENTA_GEM };
    enum GameStatus { ONGOING, WIN, LOSE };
    enum Player { PLAYER_1, PLAYER_2 };

    static const int GRID_SIZE = 8;
    static const int GEM_SIZE = 64;
    static const int GEM_SPACING = 2;
    static const int MAX_MOVES = 30;
    static const int WIN_SCORE = 5000;
    static const int WINDOW_WIDTH = 800;
    static const int WINDOW_HEIGHT = 700;

    Game();
    void reset();

    const std::vector<std::vector<GemType>>& getBoard() const { return board; }
    GameStatus status() const { return currentStatus; }
    int getPlayerScore(Player player) const { return player == PLAYER_1 ? player1Score : player2Score; }
    Player getCurrentPlayer() const { return currentPlayer; }
    int getMovesLeft() const { return movesLeft; }
    int getSelectedRow() const { return selectedRow; }
    int getSelectedCol() const { return selectedCol; }
    bool isAnimating() const { return m_isAnimating; }
    bool isCascading() const { return m_isCascading; }
    bool isDropping() const { return m_isDropping; }
    void setCascading(bool cascading) { m_isCascading = cascading; }

    void setSelectedGem(int row, int col);
    void endTurn();
    void update(float deltaTime);
    void play(int r1, int c1, int r2, int c2);
    void draw(SDL_Renderer* renderer, SDL_Texture* blueTex, SDL_Texture* greenTex, SDL_Texture* magentaTex, SDL_Texture* redTex, SDL_Texture* yellowTex);

private:
    std::vector<std::vector<GemType>> board;
    GameStatus currentStatus;
    int selectedRow;
    int selectedCol;
    float swapAnimationProgress;
    bool m_isAnimating;
    bool m_isCascading;
    bool m_isDropping;
    bool m_isRefilling;
    float dropAnimationProgress;
    std::vector<std::pair<int, int>> droppedGems;
    std::vector<std::vector<int>> gemDropStartRow;
    int swapR1, swapC1, swapR2, swapC2;
    Player currentPlayer;
    int player1Score;
    int player2Score;
    int movesLeft;
    int writeRow; // Helper for dropGems

    void initializeBoard();
    void animateSwap(int r1, int c1, int r2, int c2);
    bool isValidSwap(int r1, int c1, int r2, int c2) const;
    bool checkPotentialMatch(int r1, int c1, int r2, int c2) const;
    void swapGems(int r1, int c1, int r2, int c2);
    bool hasMatches() const;
    int clearMatches();
    void addScore(int matches);
    void dropGems();
    void refillBoard();
    bool hasValidMoves() const;
    SDL_Color getGemColor(GemType type) const;
    void drawGem(SDL_Renderer* renderer, GemType type, int x, int y, SDL_Texture* blueTex, SDL_Texture* greenTex, SDL_Texture* magentaTex, SDL_Texture* redTex, SDL_Texture* yellowTex) const;

    void drawCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color) const;
    void drawTriangle(SDL_Renderer* renderer, int x, int y, int size, SDL_Color color) const;
    void drawOctagon(SDL_Renderer* renderer, int x, int y, int size, SDL_Color color) const;
};