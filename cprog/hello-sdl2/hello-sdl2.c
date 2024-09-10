//======================================================================
// This program draws the letters S, D, and L on the screen using
// rectangles.
//
// 2024-09-09 Mon
// Dov Grobgeld <dov.grobgeld@gmail.com>
//----------------------------------------------------------------------

#include <SDL2/SDL.h>
#include <stdio.h>

// Screen dimensions for r36s
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Define the letters using a 5x5 grid
int S[5][5] = {
    {1, 1, 1, 1, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 1, 0},
    {0, 0, 0, 1, 0},
    {1, 1, 1, 1, 0}
};

int D[5][5] = {
    {1, 1, 1, 0, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 0, 0, 1, 0},
    {1, 1, 1, 0, 0}
};

int L[5][5] = {
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0},
    {1, 1, 1, 0, 0}
};

// Function to draw a letter
void drawLetter(SDL_Renderer* renderer, int letter[5][5], int xOffset, int yOffset, int cellSize, int red, int green, int blue)
{
    // Set draw color to black
    SDL_SetRenderDrawColor(renderer, red, green, blue, 255);


    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            if (letter[y][x]) {
                SDL_Rect rect = { xOffset + x * cellSize, yOffset + y * cellSize, cellSize, cellSize };
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }
}

int main(int argc, char* args[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("SDL in Rectangles", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Calculate cell size and offsets to center the text
    int totalWidth = 15; // 3 letters * 5 cells each
    int cellSize = SCREEN_WIDTH / totalWidth;
    int xOffset = (SCREEN_WIDTH - totalWidth * cellSize) / 2 + cellSize;
    int yOffset = (SCREEN_HEIGHT - 5 * cellSize) / 2;

    // Main loop flag
    int quit = 0;

    // Event handler
    SDL_Event e;

    // Store the initial time
    Uint32 startTime = SDL_GetTicks();

    // Main loop
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        // Clear screen to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw the letters
        drawLetter(renderer, S, xOffset, yOffset, cellSize, 255, 0, 0);
        drawLetter(renderer, D, xOffset + 5 * cellSize, yOffset, cellSize, 0, 255, 0);
        drawLetter(renderer, L, xOffset + 10 * cellSize, yOffset, cellSize, 0, 0, 255);

        // Update screen
        SDL_RenderPresent(renderer);

        // Delay to maintain 60 frames per second
        Uint32 elapsedTime = SDL_GetTicks() - startTime;
        if (elapsedTime < 1000 / 60) 
            SDL_Delay(1000 / 60 - elapsedTime);

        // If five seconds have passed, exit the loop
        if (SDL_GetTicks() - startTime > 30000) 
            quit = 1;
    }

    // Destroy window and renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}
