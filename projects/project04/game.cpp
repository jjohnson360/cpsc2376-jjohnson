// game.cpp

#include "Game.h"

#include <cstdlib>
#include <ctime>
#include <algorithm>
//#include <SDL2/SDL_gfxPrimitives.h> // Still included in case you use other gfx primitives
#include <cmath>
#include <iostream> // Include for debugging print statements

Game::Game() : m_isCascading(false), m_isDropping(false), m_isRefilling(false), // Initialize new members
dropAnimationProgress(0.0f), selectedRow(-1), selectedCol(-1),
swapAnimationProgress(0.0f), m_isAnimating(false),
player1Score(0), player2Score(0), movesLeft(MAX_MOVES),
currentStatus(ONGOING), currentPlayer(PLAYER_1) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    reset();
}

void Game::reset() {
    board.assign(GRID_SIZE, std::vector<GemType>(GRID_SIZE, EMPTY)); // Use assign to resize and initialize
    selectedRow = -1;
    selectedCol = -1;
    swapAnimationProgress = 0.0f;
    m_isAnimating = false;
    m_isCascading = false;
    m_isDropping = false;
    m_isRefilling = false;
    dropAnimationProgress = 0.0f;
    droppedGems.clear();
    gemDropStartRow.assign(GRID_SIZE, std::vector<int>(GRID_SIZE, -1)); // Reset drop start rows
    currentPlayer = PLAYER_1;
    player1Score = 0;
    player2Score = 0;
    movesLeft = MAX_MOVES;
    currentStatus = ONGOING;
    initializeBoard();
}

void Game::initializeBoard() {
    // Fill the board with random gems, ensuring no immediate matches
    for (int r = 0; r < GRID_SIZE; ++r) {
        for (int c = 0; c < GRID_SIZE; ++c) {
            GemType type;
            do {
                type = static_cast<GemType>(1 + std::rand() % 5); // Assign random gem type (1-5)
            } while (
                // Check for vertical match above
                (r >= 2 && board[r - 1][c] != EMPTY && board[r - 1][c] == type && board[r - 2][c] == type) ||
                // Check for horizontal match left
                (c >= 2 && board[r][c - 1] != EMPTY && board[r][c - 1] == type && board[r][c - 2] == type)
                );
            board[r][c] = type;
        }
    }
}

// Function definition for setSelectedGem - ADDED
void Game::setSelectedGem(int row, int col) {
    selectedRow = row;
    selectedCol = col;
}

void Game::play(int r1, int c1, int r2, int c2) {
    if (m_isAnimating || m_isCascading || m_isDropping || m_isRefilling || !isValidSwap(r1, c1, r2, c2) ||
        !checkPotentialMatch(r1, c1, r2, c2)) {
        selectedRow = -1; // Deselect if the play is invalid
        selectedCol = -1;
        return;
    }

    // Store swap coordinates and start animation
    swapR1 = r1;
    swapC1 = c1;
    swapR2 = r2;
    swapC2 = c2;
    swapAnimationProgress = 0.0f;
    m_isAnimating = true;
    m_isCascading = false; // Reset cascading on a new player initiated swap
    m_isDropping = false; // Ensure dropping is false at the start of a new play
    m_isRefilling = false; // Ensure refilling is false

    selectedRow = -1; // Deselect gems when a swap is initiated
    selectedCol = -1;
}

void Game::animateSwap(int r1, int c1, int r2, int c2) {
    // Animation logic handled in update()
    // This function is called internally within play, but the animation progress is tracked in update
}

void Game::update(float deltaTime) {
    if (m_isAnimating) {
        swapAnimationProgress += deltaTime * 4.0f; // Speed of swap animation
        if (swapAnimationProgress >= 1.0f) {
            swapGems(swapR1, swapC1, swapR2, swapC2);

            // After swapping, check for matches
            int cleared = clearMatches();
            if (cleared > 0) {
                addScore(cleared);
                m_isAnimating = false; // End swap animation
                m_isCascading = true; // Start cascading process
                m_isDropping = true; // Immediately start dropping if matches were cleared
                dropGems(); // Initiate the drop
            }
            else {
                // If no match, swap back
                swapGems(swapR1, swapC1, swapR2, swapC2);
                m_isAnimating = false; // End swap animation
                endTurn(); // End the turn if no match
            }
            swapAnimationProgress = 0.0f; // Reset animation progress
            // selectedRow and selectedCol are already reset in play() or endTurn()
        }
    }
    else if (m_isCascading) {
        if (m_isDropping) {
            dropAnimationProgress += deltaTime * 4.0f; // Speed of drop animation
            if (dropAnimationProgress >= 1.0f) {
                m_isDropping = false; // End dropping animation
                m_isRefilling = true; // Move to refilling state
                refillBoard(); // Refill happens instantly
                dropAnimationProgress = 0.0f; // Reset drop animation progress
                droppedGems.clear(); // Clear dropped gems list after animation
                gemDropStartRow.assign(GRID_SIZE, std::vector<int>(GRID_SIZE, -1)); // Reset drop start rows
            }
        }
        else if (m_isRefilling) {
            m_isRefilling = false; // End refilling state

            // After refilling, check for new matches
            int cleared = clearMatches();
            if (cleared > 0) {
                addScore(cleared);
                m_isCascading = true; // Continue cascading
                m_isDropping = true; // Start dropping for the next cascade
                dropGems(); // Initiate the next drop
            }
            else {
                m_isCascading = false; // End cascading if no more matches
                endTurn(); // End the turn
            }
        }
        else {
            // If not dropping or refilling but still in cascading state,
            // this means a cascade just finished, check for new matches immediately.
            int cleared = clearMatches();
            if (cleared > 0) {
                addScore(cleared);
                m_isDropping = true; // Start dropping for the next cascade
                dropGems(); // Initiate the next drop
            }
            else {
                m_isCascading = false; // End cascading if no more matches
                endTurn(); // End the turn
            }
        }
    }
}

void Game::endTurn() {
    // Only decrement moves if the game is still ongoing and a valid move was attempted
    // (This check needs to be done where play is called, or refine play/update logic)

    // Check game over conditions only after all animations and cascades are complete
    if (!m_isAnimating && !m_isCascading && !m_isDropping && !m_isRefilling) {
        // Decrement moves here, after a complete turn cycle (including cascades)
        // This assumes a swap attempt (whether successful or not) consumes a move
        // If only successful swaps consume a move, adjust where movesLeft is decremented.
        if (currentStatus == ONGOING) {
            movesLeft--;
        }


        if (player1Score >= WIN_SCORE || player2Score >= WIN_SCORE) {
            currentStatus = WIN;
        }
        else if (movesLeft <= 0) { // Check moves left explicitly
            if (!hasValidMoves()) {
                currentStatus = LOSE; // Set status to LOSE if out of moves and no valid moves left
            }
            else {
                // If out of moves but valid moves exist, the game might continue until no valid moves or score reached
                // Depending on game rules, might be instant lose or play until no moves.
                // Current logic: lose if out of moves OR no valid moves.
                currentStatus = LOSE;
            }
        }
        else if (!hasValidMoves()) { // Check for valid moves remaining
            currentStatus = LOSE; // Lose if no valid moves left (regardless of movesLeft count)
        }
        else {
            // Switch player only if the game is still ongoing and the turn ended successfully (or failed swap)
            if (currentStatus == ONGOING) {
                currentPlayer = (currentPlayer == PLAYER_1) ? PLAYER_2 : PLAYER_1;
            }
        }
        selectedRow = -1; // Ensure gems are deselected at the end of a turn cycle
        selectedCol = -1;
    }
}

bool Game::isValidSwap(int r1, int c1, int r2, int c2) const {
    // Check boundary conditions first
    if (r1 < 0 || r1 >= GRID_SIZE || c1 < 0 || c1 >= GRID_SIZE ||
        r2 < 0 || r2 >= GRID_SIZE || c2 < 0 || c2 >= GRID_SIZE) {
        return false;
    }

    // Check if the swap is between adjacent gems
    int dr = std::abs(r1 - r2);
    int dc = std::abs(c1 - c2);
    return (dr + dc == 1);
}

bool Game::checkPotentialMatch(int r1, int c1, int r2, int c2) const {
    // Create a temporary copy of the board
    Game temp = *this;

    // Perform the hypothetical swap on the temporary board
    temp.swapGems(r1, c1, r2, c2);

    // Check if the temporary board has any matches after the swap
    return temp.hasMatches();
}

void Game::swapGems(int r1, int c1, int r2, int c2) {
    std::swap(board[r1][c1], board[r2][c2]);
}

bool Game::hasMatches() const {
    // Check for horizontal matches
    for (int r = 0; r < GRID_SIZE; ++r) {
        for (int c = 0; c < GRID_SIZE - 2; ++c) {
            GemType type = board[r][c];
            if (type != EMPTY &&
                type == board[r][c + 1] &&
                type == board[r][c + 2]) {
                return true;
            }
        }
    }

    // Check for vertical matches
    for (int c = 0; c < GRID_SIZE; ++c) {
        for (int r = 0; r < GRID_SIZE - 2; ++r) {
            GemType type = board[r][c];
            if (type != EMPTY &&
                type == board[r + 1][c] &&
                type == board[r + 2][c]) {
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

    // Mark horizontal matches
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

    // Mark vertical matches (also mark gems already marked by horizontal matches)
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

    // Clear marked gems and count them
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
    // Basic scoring: 100 points per cleared gem
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
    gemDropStartRow.assign(GRID_SIZE, std::vector<int>(GRID_SIZE, -1)); // Reset drop start rows

    bool anyGemsDropped = false;

    // For each column, move non-empty gems down to fill empty spaces
    for (int c = 0; c < GRID_SIZE; ++c) {
        int writeRow = GRID_SIZE - 1; // Start from the bottom
        for (int r = GRID_SIZE - 1; r >= 0; --r) {
            if (board[r][c] != EMPTY) {
                if (writeRow != r) {
                    // Record the original position for animation
                    gemDropStartRow[writeRow][c] = r;

                    // Move gem down
                    board[writeRow][c] = board[r][c];
                    board[r][c] = EMPTY;

                    // Add to dropped gems list for animation
                    // droppedGems.push_back({writeRow, c}); // Store destination
                    anyGemsDropped = true; // Mark that a gem was moved
                }
                writeRow--; // Move the write position up
            }
        }
    }

    // Populate droppedGems based on gemDropStartRow for animation
    for (int r = 0; r < GRID_SIZE; ++r) {
        for (int c = 0; c < GRID_SIZE; ++c) {
            if (gemDropStartRow[r][c] != -1) {
                droppedGems.push_back({ r, c }); // Store the destination (current) position
            }
        }
    }


    // If any gems were moved, start the dropping animation
    if (anyGemsDropped) {
        m_isDropping = true;
        dropAnimationProgress = 0.0f;
    }
    else {
        // If no gems dropped, immediately move to refilling (though refill is instant)
        m_isDropping = false;
        m_isRefilling = true; // This will cause refillBoard to be called in the next update
    }
}

void Game::refillBoard() {
    // Fill in the empty spaces from the top
    for (int c = 0; c < GRID_SIZE; ++c) {
        for (int r = 0; r < GRID_SIZE; ++r) {
            if (board[r][c] == EMPTY) {
                board[r][c] = static_cast<GemType>(1 + std::rand() % 5); // Assign random gem type (1-5)
            }
        }
    }
    // No need to clear droppedGems or gemDropStartRow here, done after drop animation
}


bool Game::hasValidMoves() const {
    // Check for potential matches after any possible swap
    for (int r = 0; r < GRID_SIZE; ++r) {
        for (int c = 0; c < GRID_SIZE; ++c) {
            // Check swap with gem below
            if ((r + 1 < GRID_SIZE && checkPotentialMatch(r, c, r + 1, c))) {
                return true;
            }
            // Check swap with gem to the right
            if (c + 1 < GRID_SIZE && checkPotentialMatch(r, c, r, c + 1)) {
                return true;
            }
        }
    }
    return false; // No valid moves found
}


SDL_Color Game::getGemColor(GemType type) const {
    switch (type) {
    case RED_GEM: return { 255, 0, 0, 255 };
    case GREEN_GEM: return { 0, 255, 0, 255 };
    case YELLOW_GEM: return { 255, 255, 0, 255 };
    case BLUE_GEM: return { 0, 0, 255, 255 };
    case MAGENTA_GEM: return { 255, 0, 255, 255 };
    default:
        return { 50, 50, 50, 255 }; // Color for EMPTY or unknown type
    }
}

void Game::drawGem(SDL_Renderer* renderer, GemType type, int x, int y,
    SDL_Texture* blueTex, SDL_Texture* greenTex, SDL_Texture* magentaTex,
    SDL_Texture* redTex, SDL_Texture* yellowTex) const {

    SDL_Texture* texture = nullptr;
    SDL_Rect destRect = { x, y, GEM_SIZE, GEM_SIZE };

    switch (type) {
    case RED_GEM: texture = redTex; break;
    case GREEN_GEM: texture = greenTex; break;
    case YELLOW_GEM: texture = yellowTex; break;
    case BLUE_GEM: texture = blueTex; break;
    case MAGENTA_GEM: texture = magentaTex; break;
    case EMPTY: texture = nullptr; break; // Don't draw anything for empty gems
    default: texture = nullptr; break;
    }

    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    }
}


void Game::draw(SDL_Renderer* renderer, SDL_Texture* blueTex, SDL_Texture*
    greenTex,
    SDL_Texture* magentaTex, SDL_Texture* redTex, SDL_Texture* yellowTex) {

    int boardHeight = GRID_SIZE * (GEM_SIZE + GEM_SPACING);
    int boardWidth = GRID_SIZE * (GEM_SIZE + GEM_SPACING);
    int boardX = (WINDOW_WIDTH - boardWidth) / 2;
    int boardY = 120 + (WINDOW_HEIGHT - 120 - boardHeight) / 2; // Uses UL_HEADER_HEIGHT implicitly from main.cpp logic


    for (int r = 0; r < GRID_SIZE; ++r) {
        for (int c = 0; c < GRID_SIZE; ++c) {
            int x = boardX + c * (GEM_SIZE + GEM_SPACING);
            int y = boardY + r * (GEM_SIZE + GEM_SPACING);

            GemType gemToDraw = board[r][c]; // Default to drawing the gem currently in the board data

            // Animate swap
            if (m_isAnimating && ((r == swapR1 && c == swapC1) || (r == swapR2 && c ==
                swapC2))) {
                // Calculate the displacement based on animation progress
                float progress = swapAnimationProgress;

                int fromR = (r == swapR1 && c == swapC1) ? swapR1 : swapR2;
                int fromC = (r == swapR1 && c == swapC1) ? swapC1 : swapC2;
                int toR = (r == swapR1 && c == swapC1) ? swapR2 : swapR1;
                int toC = (r == swapR1 && c == swapC1) ? swapC2 : swapC1;


                // Calculate the starting pixel position of the gem before the swap
                int startX = boardX + fromC * (GEM_SIZE + GEM_SPACING);
                int startY = boardY + fromR * (GEM_SIZE + GEM_SPACING);

                // Calculate the ending pixel position of the gem after the swap
                int endX = boardX + toC * (GEM_SIZE + GEM_SPACING);
                int endY = boardY + toR * (GEM_SIZE + GEM_SPACING);

                // Calculate the current interpolated position
                int currentX = startX + static_cast<int>((endX - startX) * progress);
                int currentY = startY + static_cast<int>((endY - startY) * progress);

                // Draw the gem at its interpolated position during the swap
                // To draw the correct gem type during the swap, we need the gem type *before* the swap.
                // Since the board state is updated *after* animation progress >= 1.0,
                // board[r][c] should still hold the gem type from *before* the swap for this frame.
                drawGem(renderer, gemToDraw, currentX, currentY, blueTex, greenTex,
                    magentaTex, redTex, yellowTex);

            }
            // Animate dropping
            else if (m_isDropping) {
                bool isThisGemDropping = false;
                int startDropRow = -1;
                // Check if the gem that *ends up* at (r, c) after dropping started from a higher row
                if (gemDropStartRow[r][c] != -1) {
                    isThisGemDropping = true;
                    startDropRow = gemDropStartRow[r][c];
                }

                if (isThisGemDropping && startDropRow != -1) {
                    // The gem at board[r][c] is the one that dropped to this position.
                    // It started from startDropRow, column c.
                    int startY = boardY + startDropRow * (GEM_SIZE + GEM_SPACING);
                    int endY = y; // The destination is the current row's y coordinate
                    int currentY = startY + static_cast<int>((endY - startY) *
                        dropAnimationProgress);

                    // Draw the gem at its interpolated position during the drop
                    drawGem(renderer, gemToDraw, x, currentY, blueTex, greenTex, magentaTex,
                        redTex, yellowTex);
                }
                else {
                    // If a gem is not involved in the current drop animation, draw it stationary
                    if (gemToDraw != EMPTY) {
                        drawGem(renderer, gemToDraw, x, y, blueTex, greenTex, magentaTex,
                            redTex, yellowTex);
                    }
                }
            }
            // Draw stationary gems
            else {
                // Only draw if the gem is not EMPTY and not currently involved in swap/drop animation
                if (gemToDraw != EMPTY) {
                    drawGem(renderer, gemToDraw, x, y, blueTex, greenTex, magentaTex, redTex,
                        yellowTex);
                }
            }

            // Draw selected outline - MOVED INSIDE THE LOOPS
            if (r == selectedRow && c == selectedCol && !m_isAnimating && !m_isDropping
                && !m_isCascading && !m_isRefilling && board[r][c] != EMPTY) { // Added checks for all animation states and non-empty
                SDL_Rect outline = { x - 2, y - 2, GEM_SIZE + 4, GEM_SIZE + 4 };
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderDrawRect(renderer, &outline);
            }
        }
    }
}

// Removed unused drawing functions
// void Game::drawCircle(SDL_Renderer* renderer, int x, int y, int radius, SDL_Color color) const {}
// void Game::drawTriangle(SDL_Renderer* renderer, int x, int y, int size, SDL_Color color) const {}
// void Game::drawOctagon(SDL_Renderer* renderer, int x, int y, int size, SDL_Color color) const {}
