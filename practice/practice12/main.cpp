#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <iostream>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Bouncing Rectangle",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect rect = { 100, 100, 50, 50 };
    int dx = 4, dy = 3;

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
                running = false;
        }

        rect.x += dx;
        rect.y += dy;

        if (rect.x < 0 || rect.x + rect.w > 800) dx *= -1;
        if (rect.y < 0 || rect.y + rect.h > 600) dy *= -1;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // black background
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red rectangle
        SDL_RenderFillRect(renderer, &rect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}