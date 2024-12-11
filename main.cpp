#include <SDL2/SDL.h>
#include <cstdlib> // for rand()
#include <ctime>   // for seeding rand()
#include <iostream>
#include <chrono>  // for measuring time

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "Random Pixel Drawer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, // width
        600, // height
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Seed the random number generator
    std::srand(std::time(0));

    // Main loop
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Start timing
        auto start = std::chrono::high_resolution_clock::now();

        // Draw random pixels
        for (int y = 0; y < 600; ++y) {
            for (int x = 0; x < 800; ++x) {
                int r = std::rand() % 256;
                int g = std::rand() % 256;
                int b = std::rand() % 256;

                // Set the drawing color
                SDL_SetRenderDrawColor(renderer, r, g, b, 255);

                // Draw the pixel
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }

        // Stop timing
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << "Drawing execution time: " << duration.count() << " ms" << std::endl;

        // Present the frame
        SDL_RenderPresent(renderer);

        // Delay to visualize the frame
        SDL_Delay((100 - duration.count() > 0) ? 100 - duration.count() : 0);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
