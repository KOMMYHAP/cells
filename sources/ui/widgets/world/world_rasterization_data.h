#pragma once

struct WorldRasterizationData {
    std::byte* rawPixels;
    uint32_t pitch { 0 };
    size_t totalPixelsCount { 0 };
    const SDL_PixelFormat* pixelFormat { nullptr };
};