#pragma once

struct WorldRasterizationData {
    std::byte* rawPixelData;
    int64_t pixelDataBytesCount { 0 };
    int32_t pitch { 0 };
    const SDL_PixelFormat* pixelFormat { nullptr };
};