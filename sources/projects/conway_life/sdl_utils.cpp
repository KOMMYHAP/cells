#include "sdl_utils.h"

uint32_t PackColor(SDL_Color color)
{
    uint32_t packedColor { 0 };
    packedColor |= color.r;
    packedColor <<= 8;
    packedColor |= color.g;
    packedColor <<= 8;
    packedColor |= color.b;
    packedColor <<= 8;
    packedColor |= color.a;
    return packedColor;
}

SDL_Color UnpackColor(uint32_t packedColor)
{
    SDL_Color color {};
    color.a = static_cast<uint8_t>(packedColor & 255);
    packedColor >>= 8;
    color.b = static_cast<uint8_t>(packedColor & 255);
    packedColor >>= 8;
    color.g = static_cast<uint8_t>(packedColor & 255);
    packedColor >>= 8;
    color.r = static_cast<uint8_t>(packedColor & 255);
    return color;
}