#include "generated/auto_update_emitter_system.h"

#include "components/generated/auto_cell_emitter.h"
#include "components/generated/auto_cell_particle.h"

void UpdateEmitterSystem::DoProcessComponents(EcsEntity id, CellEmitter& cellEmitter)
{
    if (cellEmitter.emitterLifetime <= 0) {
        _ecsWorld->remove<CellEmitter>(id);
        return;
    }

    cellEmitter.emitterLifetime -= 1;
}