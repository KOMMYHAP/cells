#include "generated/auto_particles_update_stats_system.h"

#include "components/generated/auto_particle_lifetime.h"
#include "game_controller.h"

void ParticlesUpdateStatsSystem::DoSystemUpdate()
{
    const int32_t particlesCount = static_cast<int32_t>(_ecsWorld->view<ParticleLifetime>().size());
    _gameController->SetParticlesCount(particlesCount);
}