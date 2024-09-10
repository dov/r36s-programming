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

int main(int argc, char* args[])
{
    // Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0 ) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Check for joysticks
    SDL_Joystick *joystick = NULL;
    if (SDL_NumJoysticks() < 1) {
        printf( "Warning: No joysticks connected!\n" );
        return EXIT_FAILURE;
    }
    else {
        // Load joystick
        joystick = SDL_JoystickOpen( 0 );
        if (joystick == NULL) {
            printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
            return EXIT_FAILURE;
        }
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

    // Main loop flag
    int quit = 0;

    // Event handler
    SDL_Event e;

    // The keymask contains in bit 0 - Fn, and in bit 1 - Start
    Uint32 keymask = 0;

    // Main loop
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_JOYBUTTONDOWN)
            {
                switch(e.jbutton.button)
                {
                case 16: // Fn
                    keymask = keymask | 1; // turn on bit 0
                    break;
                case 13: // Start
                    keymask = keymask | 2; // turn on bit 1
                    break;
                default:
                    break;
                }
                if (keymask == 3) // Fn+Start
                    quit = SDL_TRUE;

                printf("Button down. Button=%d\n", e.jbutton.button);
            }
            else if (e.type == SDL_JOYBUTTONUP)
            {
                switch(e.jbutton.button)
                {
                case 16: // Fn
                    keymask = keymask & 0xfe; // turn off bit 0
                    break;
                case 13: // Start
                    keymask = keymask & 0xfd; // turn off bit 1
                    break;
                default:
                    break;
                }
                printf("Button up. Button=%d\n", e.jbutton.button);
            }
            else if (e.type == SDL_JOYAXISMOTION)
            {
                printf("Axis motion. Axis=%d Value=%d\n", e.jaxis.axis, e.jaxis.value);
            }

            // This is not supported on the 36s
            else if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

    }

    // Destroy window and renderer
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}
