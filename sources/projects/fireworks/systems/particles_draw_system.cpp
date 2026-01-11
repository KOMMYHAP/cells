#include "generated/auto_particles_draw_system.h"

#include "SDL3/SDL_pixels.h"
#include "components/generated/auto_cell_position.h"
#include "components/generated/auto_particle_color.h"
#include "components/generated/auto_particle_position.h"
#include "sdl_utils.h"
#include "widgets/world/world_rasterization_target.h"

void ParticlesDrawSystem::DoProcessComponents(EcsEntity /*id*/, const ParticlePosition& particlePosition, const ParticleColor& particleColor)
{
    const SDL_Color color = UnpackColor(particleColor.color);
    const CellPosition position {
        static_cast<int16_t>(particlePosition.x),
        static_cast<int16_t>(particlePosition.y),
    };
    _worldRasterizationTarget->Set(position, color);
}