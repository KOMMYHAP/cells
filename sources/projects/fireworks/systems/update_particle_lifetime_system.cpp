#include "SDL3/SDL_pixels.h"
#include "components/generated/auto_emitter.h"
#include "components/generated/auto_particle_lifetime.h"
#include "generated/auto_update_particle_lifetime_system.h"
#include "game_config.h"

void UpdateParticleLifetimeSystem::DoProcessComponents(EcsEntity id, const Emitter& emitter, ParticleLifetime& particleLifetime)
{
    if (particleLifetime.lifetime <= 0) {
        _ecsWorld->destroy(id);
        return;
    }

    const float t = static_cast<float>(particleLifetime.initialLifetime - particleLifetime.lifetime) / static_cast<float>(particleLifetime.initialLifetime);
    cellParticle.lifetime -= 1;
}