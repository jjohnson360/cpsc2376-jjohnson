#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <string>
#include "Game.h"
#include <SDL2_gfxPrimitives.h>

const int UI_HEADER_HEIGHT = 120;

enum GameState {
    START_SCREEN,
    ONGOING,
    GAME_OVER,
    EXIT_MENU
};

// Helper function to load a texture
SDL_Texture* LoadTexture(SDL_Renderer* renderer, const std::string& filePath) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, filePath.c_str());
    if (!texture) {
        std::cerr << "Error loading texture '" << filePath << "': " << IMG_GetError() << std::endl;
    }
    return texture;
}

void renderText(SDL_Renderer* renderer, TTF_Font* font,
    const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    // Initialize SDL_mixer
    if (Mix_Init(MIX_INIT_OGG) == 0) {
        std::cerr << "Mix_Init error: " << Mix_GetError() << std::endl;
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio error: " << Mix_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Bejeweled - 2 Player",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        Game::WINDOW_WIDTH,
        Game::WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load textures
    SDL_Texture* blueTexture = LoadTexture(renderer, "assets/blue.png");
    SDL_Texture* greenTexture = LoadTexture(renderer, "assets/green.png");
    SDL_Texture* magentaTexture = LoadTexture(renderer, "assets/magenta.png");
    SDL_Texture* redTexture = LoadTexture(renderer, "assets/red.png");
    SDL_Texture* yellowTexture = LoadTexture(renderer, "assets/yellow.png");
    SDL_Texture* backgroundTexture = LoadTexture(renderer, "assets/background.png");
    SDL_Texture* startButtonTexture = LoadTexture(renderer, "assets/startbutton.png");
    SDL_Texture* player1WinTexture = LoadTexture(renderer, "assets/player1win.png");
    SDL_Texture* player2WinTexture = LoadTexture(renderer, "assets/player2win.png");
    SDL_Texture* exitButtonTexture = LoadTexture(renderer, "assets/exitbutton.png");
    SDL_Texture* restartButtonTexture = LoadTexture(renderer, "assets/restartbutton.png");
    SDL_Texture* exitBackgroundTexture = LoadTexture(renderer, "assets/exitbackground.png");
    SDL_Texture* startBackgroundTexture = LoadTexture(renderer, "assets/startbackground.png");
    SDL_Texture* winBackgroundTexture = LoadTexture(renderer, "assets/winbackground.png");

    Mix_Music* backgroundMusic = Mix_LoadMUS("assets/bckgdMusic.ogg");
    if (!backgroundMusic) {
        std::cerr << "Mix_LoadMUS error: " << Mix_GetError() << std::endl;
        // Handle error, but the game can still run without music for now
    }
    else {
        // Start playing background music (looping)
        Mix_PlayMusic(backgroundMusic, -1);
        Mix_VolumeMusic(50); // Set volume to 50% (range is 0-128)
    }


    // Handle texture loading errors
    if (!blueTexture || !greenTexture || !magentaTexture || !redTexture || !yellowTexture || !backgroundTexture || !startButtonTexture || !player1WinTexture || !player2WinTexture || !exitButtonTexture || !restartButtonTexture || !exitBackgroundTexture || !startBackgroundTexture || !winBackgroundTexture) {
        std::cerr << "Failed to load one or more textures. Exiting." << std::endl;
        if (blueTexture) SDL_DestroyTexture(blueTexture);
        if (greenTexture) SDL_DestroyTexture(greenTexture);
        if (magentaTexture) SDL_DestroyTexture(magentaTexture);
        if (redTexture) SDL_DestroyTexture(redTexture);
        if (yellowTexture) SDL_DestroyTexture(yellowTexture);
        if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
        if (startButtonTexture) SDL_DestroyTexture(startButtonTexture);
        if (player1WinTexture) SDL_DestroyTexture(player1WinTexture);
        if (player2WinTexture) SDL_DestroyTexture(player2WinTexture);
        if (exitButtonTexture) SDL_DestroyTexture(exitButtonTexture);
        if (restartButtonTexture) SDL_DestroyTexture(restartButtonTexture);
        if (exitBackgroundTexture) SDL_DestroyTexture(exitBackgroundTexture);
        if (startBackgroundTexture) SDL_DestroyTexture(startBackgroundTexture);
        if (winBackgroundTexture) SDL_DestroyTexture(winBackgroundTexture);
        if (backgroundMusic) Mix_FreeMusic(backgroundMusic);
        Mix_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = nullptr;
    const char* fontPaths[] = {
        "arial.ttf",
        "fonts/arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
        nullptr
    };

    for (int i = 0; fontPaths[i]; i++) {
        font = TTF_OpenFont(fontPaths[i], 24);
        if (font) break;
    }

    Game game;
    bool running = true;
    SDL_Event e;
    Uint32 lastTime = SDL_GetTicks();
    GameState currentState = START_SCREEN;

    while (running) {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;

            if (currentState == START_SCREEN) {
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;
                    int buttonWidth = 300 * 1.25; // Increased width
                    int buttonHeight = 150 * 1.25; // Increased height
                    int buttonX = (Game::WINDOW_WIDTH - buttonWidth) / 2;
                    int buttonY = (Game::WINDOW_HEIGHT - buttonHeight) / 2;
                    SDL_Rect startButtonRect = { buttonX, buttonY, buttonWidth, buttonHeight };
                    if (mouseX >= startButtonRect.x && mouseX < startButtonRect.x + startButtonRect.w &&
                        mouseY >= startButtonRect.y && mouseY < startButtonRect.y + startButtonRect.h) {
                        currentState = ONGOING;
                        game.reset();
                    }
                }
            }
            else if (currentState == ONGOING) {
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT &&
                    game.status() == Game::ONGOING && !game.isAnimating() && !game.isDropping()) {
                    int x = e.button.x;
                    int y = e.button.y;

                    int boardHeight = Game::GRID_SIZE * (Game::GEM_SIZE + Game::GEM_SPACING);
                    int boardWidth = Game::GRID_SIZE * (Game::GEM_SIZE + Game::GEM_SPACING);
                    int boardX = (Game::WINDOW_WIDTH - boardWidth) / 2;
                    int boardY = UI_HEADER_HEIGHT + (Game::WINDOW_HEIGHT - UI_HEADER_HEIGHT - boardHeight) / 2;

                    if (y >= boardY && y < boardY + boardHeight &&
                        x >= boardX && x < boardX + boardWidth) {

                        int row = (y - boardY) / (Game::GEM_SIZE + Game::GEM_SPACING);
                        int col = (x - boardX) / (Game::GEM_SIZE + Game::GEM_SPACING);

                        if (game.getSelectedRow() == -1) {
                            game.setSelectedGem(row, col);
                        }
                        else {
                            game.play(game.getSelectedRow(), game.getSelectedCol(), row, col);
                        }
                    }
                }
            }
            else if (currentState == GAME_OVER) {
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;
                    int buttonWidth = 250;
                    int buttonHeight = 60;
                    SDL_Rect winRect = { 100, 200, Game::WINDOW_WIDTH - 200, 200 };
                    int buttonYOffset = winRect.y + winRect.h + 20; // Position buttons below win image

                    SDL_Rect exitButtonPos = { Game::WINDOW_WIDTH / 2 - 100, buttonYOffset + 60, 200, 50 };
                    SDL_Rect restartButtonPos = { Game::WINDOW_WIDTH / 2 - 100, buttonYOffset, 200, 50 };


                    if (mouseX >= exitButtonPos.x && mouseX < exitButtonPos.x + exitButtonPos.w &&
                        mouseY >= exitButtonPos.y && mouseY < exitButtonPos.y + exitButtonPos.h) {
                        currentState = EXIT_MENU;
                    }
                    else if (mouseX >= restartButtonPos.x && mouseX < restartButtonPos.x + restartButtonPos.w &&
                        mouseY >= restartButtonPos.y && mouseY < restartButtonPos.y + restartButtonPos.h) {
                        currentState = START_SCREEN;
                        game.reset();
                    }
                }
            }
            else if (currentState == EXIT_MENU) {
                if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;
                    int buttonWidth = 250;
                    int buttonHeight = 60;
                    SDL_Rect exitButtonRect = { Game::WINDOW_WIDTH / 2 - 100, Game::WINDOW_HEIGHT / 2 - 60, 200, 50 };
                    SDL_Rect restartButtonRect = { Game::WINDOW_WIDTH / 2 - 100, Game::WINDOW_HEIGHT / 2 + 10, 200, 50 };

                    if (mouseX >= exitButtonRect.x && mouseX < exitButtonRect.x + exitButtonRect.w &&
                        mouseY >= exitButtonRect.y && mouseY < exitButtonRect.y + exitButtonRect.h) {
                        running = false;
                    }
                    else if (mouseX >= restartButtonRect.x && mouseX < restartButtonRect.x + restartButtonRect.w &&
                        mouseY >= restartButtonRect.y && mouseY < restartButtonRect.y + restartButtonRect.h) {
                        currentState = START_SCREEN;
                        game.reset();
                    }
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                    currentState = GAME_OVER;
                }
            }
        }

        game.update(deltaTime);

        if (currentState == ONGOING && game.status() == Game::WIN) {
            currentState = GAME_OVER;
        }
        else if (currentState == ONGOING && game.status() != Game::ONGOING && !game.isCascading()) {
            currentState = GAME_OVER;
        }

        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderClear(renderer);

        if (currentState == START_SCREEN) {
            SDL_RenderCopy(renderer, startBackgroundTexture, nullptr, nullptr);
            int buttonWidth = 300 * 1.25; // Doubled width
            int buttonHeight = 150 * 1.25; // Doubled height
            int buttonX = (Game::WINDOW_WIDTH - buttonWidth) / 2;
            int buttonY = (Game::WINDOW_HEIGHT - buttonHeight) / 2;
            SDL_Rect startButtonRect = { buttonX, buttonY, buttonWidth, buttonHeight };
            SDL_RenderCopy(renderer, startButtonTexture, nullptr, &startButtonRect);
            if (backgroundMusic) Mix_ResumeMusic();
        }
        else if (currentState == ONGOING) {
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
            game.draw(renderer, blueTexture, greenTexture, magentaTexture, redTexture, yellowTexture);
            // Draw UI elements
            SDL_Color white = { 255, 255, 255, 255 };
            SDL_Color red = { 255, 150, 150, 255 };
            SDL_Color blue = { 150, 150, 255, 255 };

            SDL_Color p1Color = (game.getCurrentPlayer() == Game::PLAYER_1) ? red : white;
            std::string p1Text = "Player 1: " + std::to_string(game.getPlayerScore(Game::PLAYER_1));
            renderText(renderer, font, p1Text, 20, 20, p1Color);

            SDL_Color p2Color = (game.getCurrentPlayer() == Game::PLAYER_2) ? blue : white;
            std::string p2Text = "Player 2: " + std::to_string(game.getPlayerScore(Game::PLAYER_2));
            int p2Width = 0;
            TTF_SizeText(font, p2Text.c_str(), &p2Width, nullptr);
            renderText(renderer, font, p2Text, Game::WINDOW_WIDTH - p2Width - 20, 20, p2Color);

            std::string targetText = "Target: " + std::to_string(Game::WIN_SCORE);
            int targetWidth = 0;
            TTF_SizeText(font, targetText.c_str(), &targetWidth, nullptr);
            renderText(renderer, font, targetText, (Game::WINDOW_WIDTH - targetWidth) / 2, 20, white);

            std::string movesText = "Moves Left: " + std::to_string(game.getMovesLeft());
            int movesWidth = 0;
            TTF_SizeText(font, movesText.c_str(), &movesWidth, nullptr);
            renderText(renderer, font, movesText, (Game::WINDOW_WIDTH - movesWidth) / 2, 50, white);
        }
        else if (currentState == GAME_OVER) {
            SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
            SDL_RenderCopy(renderer, winBackgroundTexture, nullptr, nullptr);
            game.draw(renderer, blueTexture, greenTexture, magentaTexture, redTexture, yellowTexture);

            SDL_Rect winRect = { 100, 200, Game::WINDOW_WIDTH - 200, 200 };
            SDL_Rect playerWinDrawRect = winRect; // Draw player win image at the same spot

            if (game.status() == Game::WIN) {
                if (game.getPlayerScore(Game::PLAYER_1) >= Game::WIN_SCORE) {
                    SDL_RenderCopy(renderer, player1WinTexture, nullptr, &playerWinDrawRect);
                }
                else {
                    SDL_RenderCopy(renderer, player2WinTexture, nullptr, &playerWinDrawRect);
                }
            }

            int buttonWidth = 250;
            int buttonHeight = 60;
            int buttonX = (Game::WINDOW_WIDTH - buttonWidth) / 2;
            int buttonYOffset = playerWinDrawRect.y + playerWinDrawRect.h + 20;
            SDL_Rect exitButtonPos = { buttonX, buttonYOffset + 60, buttonWidth, buttonHeight };
            SDL_RenderCopy(renderer, exitButtonTexture, nullptr, &exitButtonPos);
            SDL_Rect restartButtonPos = { buttonX, buttonYOffset, buttonWidth, buttonHeight };
            SDL_RenderCopy(renderer, restartButtonTexture, nullptr, &restartButtonPos);

            SDL_Color white = { 255, 255, 255, 255 };
        }
        else if (currentState == EXIT_MENU) {
            SDL_RenderCopy(renderer, exitBackgroundTexture, nullptr, nullptr);
            int buttonWidth = 250;
            int buttonHeight = 60;
            SDL_Rect exitButtonRect = { Game::WINDOW_WIDTH / 2 - 100, Game::WINDOW_HEIGHT / 2 - 60, 200, 50 };
            SDL_RenderCopy(renderer, exitButtonTexture, nullptr, &exitButtonRect);
            SDL_Rect restartButtonRect = { Game::WINDOW_WIDTH / 2 - 100, Game::WINDOW_HEIGHT / 2 + 10, 200, 50 };
            SDL_RenderCopy(renderer, restartButtonTexture, nullptr, &restartButtonRect);

            SDL_Color white = { 255, 255, 255, 255 };
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    if (font) TTF_CloseFont(font);
    if (backgroundMusic) Mix_FreeMusic(backgroundMusic);
    if (blueTexture) SDL_DestroyTexture(blueTexture);
    if (greenTexture) SDL_DestroyTexture(greenTexture);
    if (magentaTexture) SDL_DestroyTexture(magentaTexture);
    if (redTexture) SDL_DestroyTexture(redTexture);
    if (yellowTexture) SDL_DestroyTexture(yellowTexture);
    if (backgroundTexture) SDL_DestroyTexture(backgroundTexture);
    if (startButtonTexture) SDL_DestroyTexture(startButtonTexture);
    if (player1WinTexture) SDL_DestroyTexture(player1WinTexture);
    if (player2WinTexture) SDL_DestroyTexture(player2WinTexture);
    if (exitButtonTexture) SDL_DestroyTexture(exitButtonTexture);
    if (restartButtonTexture) SDL_DestroyTexture(restartButtonTexture);
    if (exitBackgroundTexture) SDL_DestroyTexture(exitBackgroundTexture);
    if (startBackgroundTexture) SDL_DestroyTexture(startBackgroundTexture);
    if (winBackgroundTexture) SDL_DestroyTexture(winBackgroundTexture);
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
