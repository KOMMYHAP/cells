#include "generated/auto_update_emitter_system.h"

#include "components/generated/auto_emitter.h"
#include "components/generated/auto_new_particle_tag.h"
#include "components/generated/auto_particle_color.h"
#include "game_config.h"

void UpdateEmitterSystem::DoProcessComponents(EcsEntity id, Emitter& emitter)
{
    if (emitter.framesToLive <= 0) {
        _ecsWorld->destroy(id);
        return;
    }

    emitter.framesToLive -= 1;

    if (emitter.framesToEmit <= 0) {
        EcsEntity particle = _ecsWorld->create();
        _ecsWorld->emplace<NewParticleTag>(particle);
        emitter.framesToEmit = _gameConfig->emitterFramesToEmit + emitter.generation;
        return;
    }

    emitter.framesToEmit -= 1;
}