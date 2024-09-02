#include "world_rasterization_system.h"

WorldRasterizationSystem::WorldRasterizationSystem(EcsWorld& ecsWorld)
    : SimulationEcsSystem(ecsWorld)
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
}

void WorldRasterizationSystem::ResetDestination()
{
    _rasterizationData = nullptr;
}

void WorldRasterizationSystem::DoProcessComponents(EcsEntity /*id*/, const CellType type, const CellPosition position)
{
    ASSERT(_rasterizationData != nullptr);

    // todo: ask EnTT to sort CellPosition to increase data locality?

    const size_t bpp = _rasterizationData->pixelFormat->BytesPerPixel;
    const size_t pixelDataOffset = position.y * _rasterizationData->pitch + position.x * bpp;
    ASSERT(pixelDataOffset < _rasterizationData->totalPixelsCount);
    ASSERT(pixelDataOffset + bpp <= _rasterizationData->totalPixelsCount);

    const uint32_t color = GetColor(type);
    ASSERT(bpp <= sizeof(color));
    std::memcpy(&_rasterizationData->rawPixels[pixelDataOffset], &color, bpp);
}