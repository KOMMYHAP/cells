#pragma once

struct SDL_Color;

class WorldRasterizationTarget {
public:
    WorldRasterizationTarget(SDL_Texture& texture, SDL_Color clearColor);
    ~WorldRasterizationTarget();

    const SDL_PixelFormat& GetPixelFormat() const { return *_pixelFormat; }

    void Lock();
    void Unlock();

    void Set(int32_t pixelX, int32_t pixelY, int32_t pixelsCount, SDL_Color color);

private:
    bool DebugIsLocked() const;
    uint32_t MapColorToBytes(SDL_Color color) const;

    gsl::not_null<SDL_Texture*> _texture;
    SDL_PixelFormat* _pixelFormat { nullptr };
    std::span<std::byte> _destination;
    int32_t _bytesPerPixel { 0 };
    int32_t _pitch { 0 };
    uint32_t _clearColor { 0 };
};