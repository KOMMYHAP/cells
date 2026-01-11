#include "systems/generated/auto_emitters_init_system.h"

#include "components/generated/auto_cell_position.h"
#include "components/generated/auto_emitter.h"
#include "components/generated/auto_emitter_init_request.h"
#include "components/generated/auto_emitter_random.h"
#include "components/generated/auto_particle_position.h"
#include "game_config.h"

void EmittersInitSystem::DoProcessComponents(EcsEntity id, const EmitterInitRequest& emitterInitRequest)
{
    const int32_t generation = emitterInitRequest.generation;
    const GameConfig::FireworksConfig* fireworksConfig = _gameConfig->FindFireworks(emitterInitRequest.generation);
    _ecsWorld->remove<EmitterInitRequest>(id);

    if (!fireworksConfig) {
        ASSERT(false, "Invalid fireworks emitter's epoch!");
        return;
    }

    _ecsWorld->emplace<Emitter>(id,
        fireworksConfig->emitterFramesToEmit,
        0, //< emit new particle as soon as possible
        fireworksConfig->emitterFramesToLive,
        generation);
    _ecsWorld->emplace<ParticlePosition>(id, emitterInitRequest.x, emitterInitRequest.y);
}