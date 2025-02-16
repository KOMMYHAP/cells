#include "world_rasterization_system.h"
#include "world_rasterization_data.h"

WorldRasterizationSystem::WorldRasterizationSystem(EcsWorld& ecsWorld, uint16_t cellSize)
    : SimulationEcsSystem(ecsWorld)
    , _cellSizeInPixels(NarrowCast<int16_t>(cellSize))
{
}

uint32_t WorldRasterizationSystem::GetColor(const CellType type) const
{
    const auto* pixelFormat = _rasterizationData->pixelFormat;
    switch (type) {
    case CellType::Unit:
        return SDL_MapRGB(pixelFormat, 255, 0, 0);
    case CellType::Food:
        return SDL_MapRGB(pixelFormat, 255, 255, 0);
    case CellType::Wall:
        return SDL_MapRGB(pixelFormat, 255, 255, 255);
    case CellType::Dummy:
        return SDL_MapRGB(pixelFormat, 0, 0, 0);
    }
    return SDL_MapRGB(pixelFormat, 255, 00, 255);
}

void WorldRasterizationSystem::SetDestination(const WorldRasterizationData& rasterizationData)
{
    _rasterizationData = &rasterizationData;

    const uint32_t color = GetColor(CellType::Dummy);
    ASSERT(_rasterizationData->pixelDataBytesCount % 4 == 0,
        "Optimized version of fucntion to clear render texture requires size of texture in bytes to be multiple of 4!");
    SDL_memset4(rasterizationData.rawPixelData, color, _rasterizationData->pixelDataBytesCount / 4);
}

void WorldRasterizationSystem::ResetDestination()
{
    _rasterizationData = nullptr;
}

void WorldRasterizationSystem::DoProcessComponents(EcsEntity /*id*/, const CellType type, const CellPosition position)
{
    if (_rasterizationData == nullptr) {
        return;
    }

    // todo: ask EnTT to sort CellPosition to increase data locality?

    const int32_t bpp = _rasterizationData->pixelFormat->BytesPerPixel;
    const uint32_t color = GetColor(type);

    auto FillPixelsRow = [&](const int32_t pixelX, const int32_t pixelY, const int32_t pixelsCount) {
        const int64_t pixelDataOffset = pixelY * _rasterizationData->pitch + pixelX * bpp;
        ASSERT(pixelDataOffset < _rasterizationData->pixelDataBytesCount, "Pixel data offset is out of bounds!");
        ASSERT(pixelDataOffset + bpp * pixelsCount <= _rasterizationData->pixelDataBytesCount, "Pixel data offset is out of bounds!");

        ASSERT(bpp == 4, "Only 4 bytes per pixel supported now!");
        const size_t bytesPerRow = pixelsCount * bpp;
        ASSERT(bytesPerRow % 4 == 0, "Pixels row should be multiple of 4 bytes now!");

        void* pixelDestination = &_rasterizationData->rawPixelData[pixelDataOffset];
        ASSERT(reinterpret_cast<uintptr_t>(pixelDestination) % 4 == 0, "memset4 requires alignment of 4-bytes");
        SDL_memset4(pixelDestination, color, bytesPerRow / 4);
    };

    const int32_t pixelX = position.x * _cellSizeInPixels;
    const int32_t pixelY = position.y * _cellSizeInPixels;
    for (int32_t rowIndex = 0; rowIndex < _cellSizeInPixels; ++rowIndex) {
        FillPixelsRow(pixelX, pixelY + rowIndex, _cellSizeInPixels);
    }
}