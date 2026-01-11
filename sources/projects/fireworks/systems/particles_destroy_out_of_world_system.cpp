#include "components/generated/auto_borders_outside_world_tag.h"
#include "generated/auto_particles_destroy_out_of_world_system.h"

void ParticlesDestroyOutOfWorldSystem::DoSystemUpdate()
{
    const auto& storage = _ecsWorld->storage<BordersOutsideWorldTag>();
    _ecsWorld->destroy(storage.begin(), storage.end());
}