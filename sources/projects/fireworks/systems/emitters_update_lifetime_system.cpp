#include "generated/auto_emitters_update_lifetime_system.h"

#include "components/generated/auto_emitter.h"
#include "components/generated/auto_particle_position.h"
#include "components/generated/auto_particle_init_request.h"

void EmittersUpdateLifetimeSystem::DoProcessComponents(EcsEntity /*id*/, const ParticlePosition& particlePosition, Emitter& emitter)
{
    emitter.framesToEmit -= 1;
    if (emitter.framesToEmit <= 0) {
        const EcsEntity particle = _ecsWorld->create();
        _ecsWorld->emplace<ParticleInitRequest>(particle, emitter.generation, particlePosition.x, particlePosition.y);
        emitter.framesToEmit = emitter.initialFramesToEmit;
    }
}