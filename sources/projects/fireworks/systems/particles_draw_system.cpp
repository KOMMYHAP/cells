#include "sdl_utils.h"
#include "SDL3/SDL_pixels.h"
#include "components/generated/auto_cell_position.h"
#include "components/generated/auto_particle_color.h"
#include "generated/auto_draw_particle_system.h"
#include "widgets/world/world_rasterization_target.h"

void DrawParticleSystem::DoProcessComponents(EcsEntity /*id*/, const CellPosition& cellPosition, const ParticleColor& particleColor)
{
    const SDL_Color color = UnpackColor(particleColor.color);
    _worldRasterizationTarget->Set(cellPosition, color);
}