#include "world_rasterization_target.h"

#include "system/sdl_panic.h"

WorldRasterizationTarget::WorldRasterizationTarget(SDL_Texture& texture, SDL_Color clearColor)
    : _texture(&texture)
    , _clearColor(MapColorToBytes(clearColor))
{
    uint32_t pixelFormat { 0 };
    if (SDL_QueryTexture(_texture, &pixelFormat, nullptr, nullptr, nullptr)) {
        PanicOnSdlError("SDL_QueryTexture"sv);
    }
    _pixelFormat = SDL_AllocFormat(pixelFormat);
    if (!_pixelFormat) {
        PanicOnSdlError("SDL_AllocFormat"sv);
    }
    _bytesPerPixel = _pixelFormat->BytesPerPixel;
}

WorldRasterizationTarget::~WorldRasterizationTarget()
{
    SDL_FreeFormat(_pixelFormat);
}

void WorldRasterizationTarget::Lock()
{
    void* pixels { nullptr };
    if (SDL_LockTexture(_texture, nullptr, &pixels, &_pitch)) {
        PanicOnSdlError("SDL_LockTexture");
    }
    int32_t height { 0 };
    if (SDL_QueryTexture(_texture.get(), nullptr, nullptr, nullptr, &height) != 0) {
        PanicOnSdlError("SDL_QueryTexture");
    }
    _destination = std::span { static_cast<std::byte*>(pixels), static_cast<size_t>(_pitch * height) };

    SDL_memset4(_destination.data(), _clearColor, _destination.size() / 4);
}

void WorldRasterizationTarget::Unlock()
{
    _destination = {};
    SDL_UnlockTexture(_texture);
}

void WorldRasterizationTarget::Set(int32_t pixelX, int32_t pixelY, int32_t pixelsCount, SDL_Color color)
{
    ASSERT(DebugIsLocked(), "Target must be locked before using!");

    const int64_t pixelDataOffset = pixelY * _pitch + pixelX * _bytesPerPixel;
    ASSERT(pixelDataOffset < static_cast<int64_t>(_destination.size()), "Pixel data offset is out of bounds!");
    ASSERT(pixelDataOffset + _bytesPerPixel * pixelsCount <= static_cast<int64_t>(_destination.size()), "Pixel data offset is out of bounds!");

    ASSERT(_bytesPerPixel == 4, "Only 4 bytes per pixel supported now!");
    const size_t bytesPerRow = pixelsCount * _bytesPerPixel;
    ASSERT(bytesPerRow % 4 == 0, "Pixels row should be multiple of 4 bytes now!");

    void* pixelDestination = &_destination[pixelDataOffset];
    ASSERT(reinterpret_cast<uintptr_t>(pixelDestination) % 4 == 0, "memset4 requires alignment of 4-bytes");
    SDL_memset4(pixelDestination, MapColorToBytes(color), bytesPerRow / 4);
}

bool WorldRasterizationTarget::DebugIsLocked() const
{
    return !_destination.empty();
}

uint32_t WorldRasterizationTarget::MapColorToBytes(SDL_Color color) const
{
    return SDL_MapRGBA(_pixelFormat, color.r, color.g, color.b, color.a);
}