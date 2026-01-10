#include "systems/generated/auto_create_emitter_system.h"

#include "components/generated/auto_emitter.h"
#include "components/generated/auto_emitter_init_request.h"
#include "game_config.h"

void CreateEmitterSystem::DoProcessComponents(EcsEntity id, const NewEmitterRequest& newEmitterRequest)
{
    const int32_t generation = newEmitterRequest.generation;
    const GameConfig::FireworksConfig* fireworksConfig = _gameConfig->FindFireworks(newEmitterRequest.generation);
    _ecsWorld->remove<NewEmitterRequest>(id);

    if (!fireworksConfig) {
        ASSERT(false, "Invalid fireworks emitter's epoch!");
        return;
    }

    Emitter& emitter = _ecsWorld->emplace<Emitter>(id);
    emitter.generation = generation;
    emitter.framesToEmit = fireworksConfig->emitterFramesToEmit;
    emitter.framesToLive = fireworksConfig->emitterFramesToLive;
}