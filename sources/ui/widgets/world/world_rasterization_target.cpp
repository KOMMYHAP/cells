#include "world_rasterization_target.h"

#include "components/generated/auto_cell_position.h"
#include "system/sdl_panic.h"

WorldRasterizationTarget::WorldRasterizationTarget(SDL_Texture &texture, SDL_Color clearColor, int32_t cellSizeInPixels)
    : _texture(&texture)
      , _clearColorBytes(MapColorToBytes(clearColor))
      , _cellSizeInPixels(cellSizeInPixels) {
}

WorldRasterizationTarget::~WorldRasterizationTarget() {
}

void WorldRasterizationTarget::Lock() {
    void *pixels{nullptr};
    if (!SDL_LockTexture(_texture, nullptr, &pixels, &_pitch)) {
        PanicOnSdlError("SDL_LockTexture");
    }
    const SDL_PropertiesID propertiesId = SDL_GetTextureProperties(_texture.get());
    const SDL_PixelFormat pixelFormat = static_cast<SDL_PixelFormat>(SDL_GetNumberProperty(propertiesId, SDL_PROP_TEXTURE_FORMAT_NUMBER,
                                                                                           static_cast<int64_t>(SDL_PixelFormat::SDL_PIXELFORMAT_UNKNOWN)));
    _pixelFormatDetails = SDL_GetPixelFormatDetails(pixelFormat);
    if (!_pixelFormatDetails) {
        PanicOnSdlError("SDL_GetPixelFormatDetails"sv);
    }
    _bytesPerPixel = _pixelFormatDetails->bytes_per_pixel;
    const int64_t height = SDL_GetNumberProperty(propertiesId, SDL_PROP_TEXTURE_HEIGHT_NUMBER, 0);
    ASSERT(height > 0, "SDL_GetNumberProperty(SDL_PROP_TEXTURE_HEIGHT_NUMBER) failed!");
    _destination = std::span{static_cast<std::byte *>(pixels), static_cast<size_t>(_pitch * height)};

    SDL_memset4(_destination.data(), _clearColorBytes, _destination.size() / 4);
}

void WorldRasterizationTarget::Unlock() {
    _destination = {};
    _bytesPerPixel = 0;
    _pixelFormatDetails = nullptr;
    SDL_UnlockTexture(_texture.get());
}

void WorldRasterizationTarget::Set(CellPosition position, SDL_Color color) {
    const int32_t pixelX = position.x * _cellSizeInPixels;
    const int32_t pixelY = position.y * _cellSizeInPixels;
    for (int32_t rowIndex = 0; rowIndex < _cellSizeInPixels; ++rowIndex) {
        SetLine(pixelX, pixelY + rowIndex, _cellSizeInPixels, color);
    }
}

void WorldRasterizationTarget::SetFilledCircle(int32_t pixelX, int32_t pixelY, float radius, SDL_Color color) {
    ASSERT(DebugIsLocked(), "Target must be locked before using!");
    if (radius < 0.0f) {
        ASSERT_FAIL("Sanity check: invalid circle radius!");
        return;
    }

    // if (radius < 1.0f) {
    //     color.a = static_cast<uint8_t>(std::lerp(SDL_ALPHA_TRANSPARENT, SDL_ALPHA_OPAQUE, radius));
    //     SetLine(pixelX, pixelY, 1, color);
    //     return;
    // }

    const int32_t radiusInPixels = static_cast<int32_t>(std::round(radius));
    for (int32_t y = pixelY - radiusInPixels; y <= pixelY + radiusInPixels; ++y) {
        const int32_t opaquePixelsCount = radiusInPixels * 2;
        SetLine(pixelX - radiusInPixels, y, opaquePixelsCount, color);
    }
}

void WorldRasterizationTarget::Set(int32_t pixelX, int32_t pixelY, int32_t pixelsCount, SDL_Color color) {
    ASSERT(DebugIsLocked(), "Target must be locked before using!");

    const int64_t pixelDataOffset = pixelY * _pitch + pixelX * _bytesPerPixel;
    ASSERT(pixelDataOffset < static_cast<int64_t>(_destination.size()), "Pixel data offset is out of bounds!");
    ASSERT(pixelDataOffset + _bytesPerPixel * pixelsCount <= static_cast<int64_t>(_destination.size()), "Pixel data offset is out of bounds!");

    ASSERT(_bytesPerPixel == 4, "Only 4 bytes per pixel supported now!");
    const size_t bytesPerRow = pixelsCount * _bytesPerPixel;
    ASSERT(bytesPerRow % 4 == 0, "Pixels row should be multiple of 4 bytes now!");

    void *pixelDestination = &_destination[pixelDataOffset];
    ASSERT(reinterpret_cast<uintptr_t>(pixelDestination) % 4 == 0, "memset4 requires alignment of 4-bytes");
    SDL_memset4(pixelDestination, MapColorToBytes(color), bytesPerRow / 4);
}

void WorldRasterizationTarget::SetLine(int32_t offsetX, int32_t y, int32_t length, SDL_Color color) {
    // for (int x = offsetX; x < offsetX + length; ++x) {
    //     if (x < 0 || x >= _texture->w) {
    //         continue;
    //     }
    //     if (y < 0 || y >= _texture->h) {
    //         continue;
    //     }
    //     const uint32_t colorBytes = MapColorToBytes(color);
    //     memcpy(&_destination[y * _pitch + x * _bytesPerPixel], &colorBytes, _bytesPerPixel);
    // }


    const bool hasYInside = y < _texture->h && y >= 0;
    if (!hasYInside) {
        return;
    }
    offsetX = std::max(offsetX, 0);
    length = std::min(length, _texture->w);
    const int32_t endX = offsetX + length;
    const bool hasXInside = offsetX < _texture->w && endX > 0;
    if (!hasXInside) {
        return;
    }


    ASSERT(DebugIsLocked(), "Target must be locked before using!");
    int64_t bytesFrom = y * _pitch + offsetX * _bytesPerPixel;
    ASSERT(bytesFrom < static_cast<int64_t>(_destination.size()), "Line's start is out of screen bounds!");
    const int64_t bytesTo = bytesFrom + length * _bytesPerPixel;
    ASSERT(bytesTo < static_cast<int64_t>(_destination.size()), "Line's end is out of screen bounds!");

    int64_t bytesCount = bytesTo - bytesFrom;
    if (_bytesPerPixel == 4) {
        const int64_t alignedBytesCount = bytesCount / 4 * 4;
        SDL_memset4(&_destination[bytesFrom], MapColorToBytes(color), alignedBytesCount / 4);
        bytesCount -= alignedBytesCount;
        bytesFrom += alignedBytesCount;
    }
    for (int i = 0; i < bytesCount; ++i) {
        const uint32_t colorBytes = MapColorToBytes(color);
        SDL_memcpy(&_destination[bytesFrom], &colorBytes, _bytesPerPixel);
        bytesFrom += _bytesPerPixel;
    }
}

bool WorldRasterizationTarget::DebugIsLocked() const {
    return !_destination.empty();
}

uint32_t WorldRasterizationTarget::MapColorToBytes(SDL_Color color) const {
    return SDL_MapRGBA(_pixelFormatDetails, nullptr, color.r, color.g, color.b, color.a);
}
