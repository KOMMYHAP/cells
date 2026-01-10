#include "generated/auto_update_emitter_system.h"

#include "components/generated/auto_emitter.h"
#include "components/generated/auto_particle_init_request.h"
#include "components/generated/auto_particle_color.h"
#include "game_config.h"

void UpdateEmitterSystem::DoProcessComponents(EcsEntity id, Emitter& emitter)
{
    emitter.framesToEmit -= 1;
    if (emitter.framesToEmit <= 0) {
        _ecsWorld->emplace<NewParticleTag>(id);
        emitter.framesToEmit = _gameConfig->;
    }
}