#include "world_rasterization_system.h"
#include "world_rasterization_target.h"

WorldRasterizationSystem::WorldRasterizationSystem(EcsWorld& ecsWorld, WorldRasterizationTarget& target)
    : SimulationEcsSystem(ecsWorld)
    , _target(&target)
{
}


void WorldRasterizationSystem::DoProcessComponents(EcsEntity /*id*/, const CellType type, const CellPosition position)
{
    // todo: ask EnTT to sort CellPosition to increase data locality?

    SDL_Color color;
    switch (type.type) {
    case CellType::Unit:
        color = SDL_Color { 255, 0, 0, 0 };
        break;
    case CellType::Food:
        color = SDL_Color { 255, 255, 0, 0 };
        break;
    case CellType::Wall:
        color = SDL_Color { 255, 255, 255, 0 };
        break;
    case CellType::Dummy:
        color = SDL_Color { 0, 0, 0, 0 };
        break;
    default:
        color = SDL_Color { 255, 00, 255, 0 };
        break;
    }
    _target->Set(position, color);
}