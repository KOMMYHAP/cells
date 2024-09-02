#pragma once

struct WorldRasterizationData {
    std::byte* rawPixelData;
    size_t pixelDataBytesCount { 0 };
    uint32_t pitch { 0 };
    const SDL_PixelFormat* pixelFormat { nullptr };
};