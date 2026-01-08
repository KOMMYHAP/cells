#include "SDL3/SDL_pixels.h"
#include "components/generated/auto_cell_position.h"
#include "components/generated/auto_field_position.h"
#include "generated/auto_draw_alive_cell_system.h"
#include "widgets/world/world_rasterization_target.h"

void DrawAliveCellSystem::DoProcessComponents(EcsEntity /*id*/, const CellPosition& cellPosition)
{
    static constexpr SDL_Color color { 0, 255, 0, SDL_ALPHA_OPAQUE };
    _worldRasterizationTarget->Set(cellPosition, color);
}