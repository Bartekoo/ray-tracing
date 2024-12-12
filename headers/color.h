#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <SDL2/SDL.h>

#include <iostream>

using color = vec3;

void write_color(const color& pixel_color, const int x, const int y, SDL_Renderer* renderer) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    SDL_SetRenderDrawColor(renderer, rbyte, gbyte, bbyte, 255);
    SDL_RenderDrawPoint(renderer, x, y);
}

#endif