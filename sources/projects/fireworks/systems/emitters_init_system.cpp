#include "systems/generated/auto_emitters_init_system.h"

#include "components/generated/auto_cell_position.h"
#include "components/generated/auto_emitter.h"
#include "components/generated/auto_emitter_init_request.h"
#include "components/generated/auto_emitter_random.h"
#include "components/generated/auto_particle_position.h"
#include "game_config.h"
#include "game_controller.h"

void EmittersInitSystem::DoProcessComponents(EcsEntity id, const EmitterInitRequest& emitterInitRequest)
{
    const int32_t generation = emitterInitRequest.generation;
    const GameConfig::FireworksConfig* fireworksConfig = _gameConfig->FindFireworks(emitterInitRequest.generation);
    _ecsWorld->remove<EmitterInitRequest>(id);

    if (!fireworksConfig) {
        _ecsWorld->destroy(id);
        return;
    }

    _ecsWorld->emplace<Emitter>(id,
        fireworksConfig->emitterParticlesPerSeconds,
        0.0f,
        fireworksConfig->emitterFramesToLive,
        generation);
    _ecsWorld->emplace<EmitterRandom>(id, _gameController->ModifyRandom().GetValue());
    _ecsWorld->emplace<ParticlePosition>(id, emitterInitRequest.x, emitterInitRequest.y);
}