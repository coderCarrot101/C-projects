#include <stdio.h>
#include <SDL2/SDL.h> // Correctly include SDL header

int main() {
    SDL_Init(SDL_INIT_VIDEO); // Initialize SDL

    SDL_Window* window = SDL_CreateWindow("events test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Surface * screen = SDL_GetWindowSurface(window);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0); // Set window background color to red
    SDL_RenderClear(renderer); // Clear the renderer

    SDL_RenderPresent(renderer); // Display the rendered content

    printf("SDL test program\n");

    int run = 0;

    while (run == 0) {
        SDL_Event ev;

        while (SDL_PollEvent(&ev)) {
            switch (ev.type) {
                case SDL_QUIT:
                    run = 1;
                    break;
            }
        }
    }
    SDL_DestroyRenderer(renderer); // Clean up renderer
    SDL_DestroyWindow(window); // Clean up window
    SDL_Quit(); // Quit SDL

    return 0;
}
