#include "world_rasterization_system.h"
#include "world_rasterization_target.h"

WorldRasterizationSystem::WorldRasterizationSystem(EcsWorld& ecsWorld, WorldRasterizationTarget& target, uint16_t cellSize)
    : SimulationEcsSystem(ecsWorld)
    , _cellSizeInPixels(NarrowCast<int16_t>(cellSize))
    , _target(&target)
{
}

SDL_Color WorldRasterizationSystem::GetColor(const CellType type) const
{
    switch (type.type) {
    case CellType::Unit:
        return SDL_Color { 255, 0, 0, 0 };
    case CellType::Food:
        return SDL_Color { 255, 255, 0, 0 };
    case CellType::Wall:
        return SDL_Color { 255, 255, 255, 0 };
    case CellType::Dummy:
        return SDL_Color { 0, 0, 0, 0 };
    default:
        return SDL_Color { 255, 00, 255, 0 };
    }
}

void WorldRasterizationSystem::DoProcessComponents(EcsEntity /*id*/, const CellType type, const CellPosition position)
{
    // todo: ask EnTT to sort CellPosition to increase data locality?
    const SDL_Color color = GetColor(type);
    const int32_t pixelX = position.x * _cellSizeInPixels;
    const int32_t pixelY = position.y * _cellSizeInPixels;
    for (int32_t rowIndex = 0; rowIndex < _cellSizeInPixels; ++rowIndex) {
        _target->Set(pixelX, pixelY + rowIndex, _cellSizeInPixels, color);
    }
}