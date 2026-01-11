#include "generated/auto_emitters_update_lifetime_system.h"

#include "components/generated/auto_emitter.h"
#include "components/generated/auto_particle_position.h"
#include "components/generated/auto_particle_init_request.h"

void EmittersUpdateLifetimeSystem::DoProcessComponents(EcsEntity id, Emitter& emitter)
{
    emitter.framesToLive -= 1;
    if (emitter.framesToLive <= 0) {
        _ecsWorld->destroy(id);
    }
}