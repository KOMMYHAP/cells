#include "generated/auto_world_rasterization_system.h"

#include "components/generated/auto_cell_position.h"
#include "components/generated/auto_cell_type.h"
#include "widgets/world/world_rasterization_target.h"

void WorldRasterizationSystem::DoProcessComponents(EcsEntity /*id*/, const CellType& type, const CellPosition& position)
{
    // todo: ask EnTT to sort CellPosition to increase data locality?

    SDL_Color color;
    switch (type.type) {
    case CellType::Unit:
        color = SDL_Color { 255, 0, 0, SDL_ALPHA_OPAQUE };
        break;
    case CellType::Food:
        color = SDL_Color { 255, 255, 0, SDL_ALPHA_OPAQUE };
        break;
    case CellType::Wall:
        color = SDL_Color { 255, 255, 255, SDL_ALPHA_OPAQUE };
        break;
    case CellType::Dummy:
        color = SDL_Color { 0, 0, 0, SDL_ALPHA_OPAQUE };
        break;
    default:
        color = SDL_Color { 255, 00, 255, SDL_ALPHA_OPAQUE };
        break;
    }

    _worldRasterizationTarget->Set(position, color);
}