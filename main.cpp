#include <SDL2/SDL.h>
#include "headers/vec3.h"
#include "headers/color.h"
#include "headers/ray.h"
#include <cstdlib> // for rand()
#include <ctime>   // for seeding rand()
#include <iostream>
#include <chrono>  // for measuring time

color ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

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

    auto aspect_ratio = 16.0/9.0;

    int window_width = 1000;
    int window_height = int(window_width / aspect_ratio);

    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "Random Pixel Drawer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        window_width, // width
        window_height, // height
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

    std::srand(std::time(0));

    const int frameDelay = 100;


    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(window_width) / window_height);
    auto camera_center = point3(0, 0, 0);

    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    auto pixel_delta_u = viewport_u / window_width;
    auto pixel_delta_v = viewport_v / window_height;

    auto viewport_upper_left = camera_center
                             - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);



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
        for (int j = 0; j < window_height; j += resolution) {
            for (int i = 0; i < window_width; i += resolution) {
                auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                auto ray_direction = pixel_center - camera_center;
                ray r(camera_center, ray_direction);

                color pixel_color = ray_color(r);
                for (int dy = 0; dy < resolution; ++dy) {
                    for (int dx = 0; dx < resolution; ++dx) {
                        write_color(pixel_color, i + dx, j + dy, renderer);
                    }
                }
            }
        }

        // Stop timing
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end - start;
        // std::cout << "Drawing execution time: " << duration.count() << " ms" << std::endl;

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
