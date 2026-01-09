#include "generated/auto_draw_cell_particle_system.h"

#include "components/generated/auto_cell_position.h"
#include "components/generated/auto_cell_particle.h"
#include "sdl_utils.h"
#include "widgets/world/world_rasterization_target.h"

void DrawCellParticleSystem::DoProcessComponents(EcsEntity /*id*/, const CellPosition& cellPosition, const CellParticle& cellParticle)
{
    const SDL_Color color = UnpackColor(cellParticle.color);
    _worldRasterizationTarget->Set(cellPosition, color);
}