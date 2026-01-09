#pragma once
#include "SDL3/SDL_pixels.h"

class GameConfig {
public:
    int32_t deathEmitterLifetime{60};
    // SDL_Color deathColor {255, 0, 0, SDL_ALPHA_OPAQUE};
    //
    int32_t birthEmitterLifetime{30};
    // SDL_Color birthColor{0, 255, 0, SDL_ALPHA_OPAQUE};

    SDL_Color emptyCellColor { 0, 0, 0, SDL_ALPHA_OPAQUE };
    SDL_Color aliveCellColor { 255, 255, 0, SDL_ALPHA_OPAQUE };
};