#include "components/generated/auto_cell_emitter.h"
#include "generated/auto_emit_new_particle_system.h"

#include "components/generated/auto_cell_particle.h"

void EmitNewParticleSystem::DoProcessComponents(EcsEntity id, const CellEmitter& cellEmitter)
{
    _ecsWorld->emplace_or_replace<CellParticle>(id, cellEmitter.particle_lifetime, cellEmitter.color_from);
}