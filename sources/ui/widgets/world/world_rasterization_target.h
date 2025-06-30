#pragma once
#include "SDL3/SDL_pixels.h"

struct CellPosition;
struct SDL_Texture;
struct SDL_PixelFormatDetails;

class WorldRasterizationTarget {
public:
    WorldRasterizationTarget(SDL_Texture& texture, SDL_Color clearColor, int32_t cellSizeInPixels);
    ~WorldRasterizationTarget();

    void Lock();
    void Unlock();

    void Set(CellPosition position, SDL_Color color);

private:
    bool DebugIsLocked() const;
    uint32_t MapColorToBytes(SDL_Color color) const;

    void Set(int32_t pixelX, int32_t pixelY, int32_t pixelsCount, SDL_Color color);

    gsl::not_null<SDL_Texture*> _texture;
    std::span<std::byte> _destination;
    const SDL_PixelFormatDetails* _pixelFormatDetails{nullptr};
    int8_t _bytesPerPixel { 0 };
    int32_t _pitch { 0 };
    uint32_t _clearColorBytes { 0 };
    int32_t _cellSizeInPixels { 0 };
};