#include <SDL2/SDL.h>
#include <cstdlib> // for rand()
#include <ctime>   // for seeding rand()
#include <iostream>
#include <chrono>  // for measuring time

int main(int argc, char* argv[]) {
    // Check if resolution is provided
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <resolution>" << std::endl;
        return -1;
    }

    // Parse resolution from argv[1]
    int resolution = std::atoi(argv[1]);
    if (resolution <= 0) {
        std::cerr << "Invalid resolution: " << argv[1] << std::endl;
        return -1;
    }

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

    // Target frame duration for 10 FPS
    const int frameDelay = 100; // milliseconds

    // Main loop
    bool running = true;
    SDL_Event event;
    while (running) {
        auto frameStart = std::chrono::high_resolution_clock::now();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        // Start timing
        auto start = std::chrono::high_resolution_clock::now();

        // Draw random pixels with specified resolution
        for (int y = 0; y < 600; y += resolution) {
            for (int x = 0; x < 800; x += resolution) {
                // Generate random RGB values
                int r = std::rand() % 256;
                int g = std::rand() % 256;
                int b = std::rand() % 256;

                // Set the drawing color
                SDL_SetRenderDrawColor(renderer, r, g, b, 255);

                // Draw the rectangle
                SDL_Rect rect = {x, y, resolution, resolution};
                SDL_RenderFillRect(renderer, &rect);
            }
        }

        // Stop timing
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        std::cout << "Drawing execution time: " << duration.count() << " ms" << std::endl;

        // Present the frame
        SDL_RenderPresent(renderer);

        // Frame rate control
        auto frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> frameDuration = frameEnd - frameStart;
        if (frameDuration.count() < frameDelay) {
            SDL_Delay(frameDelay - static_cast<int>(frameDuration.count()));
        }
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
