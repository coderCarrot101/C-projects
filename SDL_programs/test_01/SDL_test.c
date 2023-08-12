#include <stdio.h>
#include <SDL2/SDL.h> // Correctly include SDL header

int main() {
    SDL_Init(SDL_INIT_VIDEO); // Initialize SDL

    SDL_Window* window = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0); // Set window background color to white
    SDL_RenderClear(renderer); // Clear the renderer

    SDL_RenderPresent(renderer); // Display the rendered content

    printf("SDL test program\n");

    SDL_Delay(3000); // Pause execution for 3 seconds

    SDL_DestroyRenderer(renderer); // Clean up renderer
    SDL_DestroyWindow(window); // Clean up window
    SDL_Quit(); // Quit SDL

    return 0;
}
