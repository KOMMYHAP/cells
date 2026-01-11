#include "generated/auto_particles_update_lifetime_system.h"

#include "components/generated/auto_particle_lifetime.h"

void ParticlesUpdateLifetimeSystem::DoProcessComponents(EcsEntity id, ParticleLifetime& particleLifetime)
{
    if (particleLifetime.lifetime <= 0) {
        _ecsWorld->destroy(id);
        return;
    }

    particleLifetime.lifetime -= 1;
    particleLifetime.key = static_cast<float>(particleLifetime.initialLifetime - particleLifetime.lifetime) / static_cast<float>(particleLifetime.initialLifetime);
}