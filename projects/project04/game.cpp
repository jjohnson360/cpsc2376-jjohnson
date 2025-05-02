// game.cpp
#include "game.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <cmath>

Game::Game() : m_isCascading(false), m_isDropping(false), m_isRefilling(false), dropAnimationProgress(0.0f), writeRow(0) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    reset();
}

void Game::reset() {
    board =
        std::vector<std::vector<GemType>>(GRID_SIZE,
            std::vector<GemType>(GRID_SIZE, EMPTY));
    selectedRow = -1;
    selectedCol = -1;
    swapAnimationProgress = 0.0f;
    m_isAnimating = false;
    m_isCascading = false;
    m_isDropping = false;
    m_isRefilling = false;
    dropAnimationProgress = 0.0f;
    droppedGems.clear();
    gemDropStartRow.assign(GRID_SIZE, std::vector<int>(GRID_SIZE, -1));
    currentPlayer = PLAYER_1;
    player1Score = 0;
    player2Score = 0;
    movesLeft = MAX_MOVES;
    currentStatus = ONGOING;
    writeRow = 0;
    initializeBoard();
}

void Game::initializeBoard() {
    do {
        for (int r = 0; r < GRID_SIZE; ++r) {
            for (int c = 0; c < GRID_SIZE; ++c) {
                board[r][c] = static_cast<GemType>(1 + std::rand() % 5);
            }
        }
    } while (hasMatches());
}

void Game::setSelectedGem(int row, int col) {
    selectedRow = row;
    selectedCol = col;
}

void Game::play(int r1, int c1, int r2, int c2) {
    if (m_isAnimating || m_isCascading || m_isDropping || !isValidSwap(r1, c1, r2, c2) || !checkPotentialMatch(r1, c1, r2, c2)) {
        selectedRow = -1;
        selectedCol = -1;
        return;
    }
    animateSwap(r1, c1, r2, c2);
    swapR1 = r1;
    swapC1 = c1;
    swapR2 = r2;
    swapC2 = c2;
    swapAnimationProgress = 0.0f;
    m_isAnimating = true;
    m_isCascading = false; // Reset cascading on a new player initiated swap
}

void Game::animateSwap(int r1, int c1, int r2, int c2) {
    // Animation logic handled in update()
}

void Game::update(float deltaTime) {
    if (m_isAnimating) {
        swapAnimationProgress += deltaTime * 4.0f;
        if (swapAnimationProgress >= 1.0f) {
            swapGems(swapR1, swapC1, swapR2, swapC2);
            int cleared = clearMatches();
            if (cleared > 0) {
                addScore(cleared);
                m_isAnimating = false;
                m_isCascading = true;
                m_isDropping = true;  // Explicitly set to true to start dropping
                dropGems();          // Call dropGems right away
                swapAnimationProgress = 0.0f;
            }
            else {
                swapGems(swapR1, swapC1, swapR2, swapC2); // Swap back if no match
                endTurn();
                m_isAnimating = false;
                swapAnimationProgress = 0.0f;
            }
        }
    }
    else if (m_isCascading) {
        if (!m_isDropping && !m_isRefilling) {
            int cleared = clearMatches();
            if (cleared > 0) {
                addScore(cleared);
                m_isDropping = true;  // Make sure to set this flag
                dropGems();           // Call dropGems to start dropping animation
            }
            else {
                m_isCascading = false;
                endTurn();
            }
        }

        if (m_isDropping) {
            dropAnimationProgress += deltaTime * 4.0f;
            if (dropAnimationProgress >= 1.0f) {
                m_isDropping = false;
                m_isRefilling = true;
                dropAnimationProgress = 0.0f;
                refillBoard(); // Refill happens instantly for now
            }
        }

        if (m_isRefilling) {
            // Check for new matches after refilling
            m_isRefilling = false;
            if (hasMatches()) {
                m_isCascading = true;
                // Don't end turn yet - continue cascading
            }
            else {
                m_isCascading = false;
                endTurn();
            }
        }
    }
}

void Game::endTurn() {
    selectedRow = -1;
    selectedCol = -1;
    movesLeft--;
    if (player1Score >= WIN_SCORE || player2Score >= WIN_SCORE) {
        currentStatus = WIN;
    }
    else if (movesLeft <= 0 || !hasValidMoves()) {
        currentStatus = LOSE;
    }
    else if (!m_isCascading) {
        currentPlayer = (currentPlayer == PLAYER_1) ? PLAYER_2 : PLAYER_1;
    }
}

bool Game::isValidSwap(int r1, int c1, int r2, int c2) const {
    if (r1 < 0 || r1 >= GRID_SIZE || c1 < 0 || c1 >= GRID_SIZE ||
        r2 < 0 || r2 >= GRID_SIZE || c2 < 0 || c2 >= GRID_SIZE) {
        return false;
    }
    int dr = std::abs(r1 - r2);
    int dc = std::abs(c1 - c2);
    return (dr + dc == 1);
}

bool Game::checkPotentialMatch(int r1, int c1, int r2, int c2) const {
    Game temp = *this;
    temp.swapGems(r1, c1, r2, c2);
    return temp.hasMatches();
}

void Game::swapGems(int r1, int c1, int r2, int c2) {
    std::swap(board[r1][c1], board[r2][c2]);
}

bool Game::hasMatches() const {
    for (int r = 0; r < GRID_SIZE; ++r) {
        for (int c = 0; c < GRID_SIZE - 2; ++c) {
            if (board[r][c] != EMPTY &&
                board[r][c] == board[r][c + 1] &&
                board[r][c] == board[r][c + 2]) {
                return true;
            }
        }
    }
    for (int c = 0; c < GRID_SIZE; ++c) {
        for (int r = 0; r < GRID_SIZE - 2; ++r) {
            if (board[r][c] != EMPTY &&
                board[r][c] == board[r + 1][c] &&
                board[r][c] == board[r + 2][c]) {
                return true;
            }
        }
    }
    return false;
}

int Game::clearMatches() {
    std::vector<std::vector<bool>> matched(GRID_SIZE,
        std::vector<bool>(GRID_SIZE, false));
    int totalCleared = 0;

    for (int r = 0; r < GRID_SIZE; ++r) {
        for (int c = 0; c < GRID_SIZE - 2; ++c) {
            GemType type = board[r][c];
            if (type != EMPTY &&
                type == board[r][c + 1] &&
                type == board[r][c + 2]) {
                matched[r][c] = matched[r][c + 1] = matched[r][c + 2] = true;
            }
        }
    }

    for (int c = 0; c < GRID_SIZE; ++c) {
        for (int r = 0; r < GRID_SIZE - 2; ++r) {
            GemType type = board[r][c];
            if (type != EMPTY &&
                type == board[r + 1][c] &&
                type == board[r + 2][c]) {
                matched[r][c] = matched[r + 1][c] = matched[r + 2][c] = true;
            }
        }
    }

    for (int r = 0; r < GRID_SIZE; ++r) {
        for (int c = 0; c < GRID_SIZE; ++c) {
            if (matched[r][c]) {
                board[r][c] = EMPTY;
                totalCleared++;
            }
        }
    }

    return totalCleared;
}

void Game::addScore(int matches) {
    int points = matches * 100;
    if (currentPlayer == PLAYER_1) {
        player1Score += points;
    }
    else {
        player2Score += points;
    }
}

void Game::dropGems() {
    droppedGems.clear();
    gemDropStartRow.assign(GRID_SIZE, std::vector<int>(GRID_SIZE, -1));
    m_isDropping = true;
    dropAnimationProgress = 0.0f;

    // For each column
    for (int c = 0; c < GRID_SIZE; ++c) {
        int writeRow = GRID_SIZE - 1;

        // Start from bottom, move up
        for (int r = GRID_SIZE - 1; r >= 0; --r) {
            if (board[r][c] != EMPTY) {
                if (writeRow != r) {
                    // Record the original position for animation
                    gemDropStartRow[writeRow][c] = r;

                    // Move gem down
                    board[writeRow][c] = board[r][c];
                    board[r][c] = EMPTY;

                    // Add to dropped gems list for animation
                    droppedGems.push_back({ writeRow, c });
                }
                writeRow--;
            }
        }
    }

    // If no gems were moved, skip the animation
    if (droppedGems.empty()) {
        m_isDropping = false;
        m_isRefilling = true;
    }
}

void Game::refillBoard() {
    for (int c = 0; c < GRID_SIZE; ++c) {
        for (int r = 0; r < GRID_SIZE; ++r) {
            if (board[r][c] == EMPTY) {
                board[r][c] = static_cast<GemType>(1 + std::rand() % 5);
            }
        }
    }

    // Reset any animation-related variables
    droppedGems.clear();
    gemDropStartRow.assign(GRID_SIZE, std::vector<int>(GRID_SIZE, -1));
}

bool Game::hasValidMoves() const {
    for (int r = 0; r < GRID_SIZE; ++r) {
        for (int c = 0; c < GRID_SIZE; ++c) {
            if (r + 1 < GRID_SIZE && checkPotentialMatch(r, c, r + 1, c)) {
                return true;
            }
            if (c + 1 < GRID_SIZE && checkPotentialMatch(r, c, r, c + 1)) {
                return true;
            }
        }
    }
    return false;
}

SDL_Color Game::getGemColor(GemType type) const {
    switch (type) {
    case RED_GEM: return { 255, 0, 0, 255 };
    case GREEN_GEM: return { 0, 255, 0, 255 };
    case YELLOW_GEM: return { 255, 255, 0, 255 };
    case BLUE_GEM: return { 0, 0, 255, 255 };
    case MAGENTA_GEM: return { 255, 0, 255, 255 };
    default: return { 50, 50, 50, 255 };
    }
}

void Game::drawGem(SDL_Renderer* renderer, GemType type, int x, int y, SDL_Texture* blueTex, SDL_Texture* greenTex, SDL_Texture* magentaTex, SDL_Texture* redTex, SDL_Texture* yellowTex) const {
    SDL_Texture* texture = nullptr;
    SDL_Rect destRect = { x, y, GEM_SIZE, GEM_SIZE };

    switch (type) {
    case RED_GEM:
        texture = redTex;
        break;
    case GREEN_GEM:
        texture = greenTex;
        break;
    case YELLOW_GEM:
        texture = yellowTex;
        break;
    case BLUE_GEM:
        texture = blueTex;
        break;
    case MAGENTA_GEM:
        texture = magentaTex;
        break;
    case EMPTY:
    default:
        break; // Don't draw anything for empty gems
    }

    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    }
}

void Game::draw(SDL_Renderer* renderer, SDL_Texture* blueTex, SDL_Texture* greenTex, SDL_Texture* magentaTex, SDL_Texture* redTex, SDL_Texture* yellowTex) {
    int boardHeight = GRID_SIZE * (GEM_SIZE + GEM_SPACING);
    int boardWidth = GRID_SIZE * (GEM_SIZE + GEM_SPACING);
    int boardX = (WINDOW_WIDTH - boardWidth) / 2;
    int boardY = 120 + (WINDOW_HEIGHT - 120 - boardHeight) / 2;

    for (int r = 0; r < GRID_SIZE; ++r) {
        for (int c = 0; c < GRID_SIZE; ++c) {
            int x = boardX + c * (GEM_SIZE + GEM_SPACING);
            int y = boardY + r * (GEM_SIZE + GEM_SPACING);

            bool isCurrentlyDropping = false;
            int startDropRow = -1;

            if (m_isDropping) {
                // Check if this gem is one that should be animated
                for (const auto& dropped : droppedGems) {
                    if (dropped.first == r && dropped.second == c) {
                        isCurrentlyDropping = true;
                        startDropRow = gemDropStartRow[r][c];
                        break;
                    }
                }
            }

            // Animate swap
            if (m_isAnimating && ((r == swapR1 && c == swapC1) || (r == swapR2 && c == swapC2))) {
                int dx = (swapC2 - swapC1) * static_cast<int>((GEM_SIZE + GEM_SPACING) * swapAnimationProgress);
                int dy = (swapR2 - swapR1) * static_cast<int>((GEM_SIZE + GEM_SPACING) * swapAnimationProgress);
                drawGem(renderer, board[r][c], x + dx * (r == swapR1 && c == swapC1 ? 1 : -1), y + dy * (r == swapR1 && c == swapC1 ? 1 : -1), blueTex, greenTex, magentaTex, redTex, yellowTex);
            }
            // Animate dropping
            else if (m_isDropping && isCurrentlyDropping && startDropRow != -1) {
                int startY = boardY + startDropRow * (GEM_SIZE + GEM_SPACING);
                int endY = y;
                int currentY = startY + static_cast<int>((endY - startY) * dropAnimationProgress);
                drawGem(renderer, board[r][c], x, currentY, blueTex, greenTex, magentaTex, redTex, yellowTex);
            }
            // Draw stationary gems
            else {
                drawGem(renderer, board[r][c], x, y, blueTex, greenTex, magentaTex, redTex, yellowTex);
            }

            // Draw selected outline
            if (r == selectedRow && c == selectedCol && !m_isAnimating && !m_isDropping) {
                SDL_Rect outline = { x - 2, y - 2, GEM_SIZE + 4, GEM_SIZE + 4 };
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &outline);
            }
        }
    }
}

void Game::drawCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color) const {
    // You might not need these anymore if using textures, but keeping them for now
}

void Game::drawTriangle(SDL_Renderer* renderer, int x, int y, int size, SDL_Color color) const {
    // You might not need these anymore if using textures, but keeping them for now
}

void Game::drawOctagon(SDL_Renderer* renderer, int x, int y, int size, SDL_Color color) const {
    // You might not need these anymore if using textures, but keeping them for now
}
