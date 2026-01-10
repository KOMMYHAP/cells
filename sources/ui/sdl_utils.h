#pragma once
#include "SDL3/SDL_pixels.h"

uint32_t PackColor(SDL_Color color);
SDL_Color UnpackColor(uint32_t packedColor);