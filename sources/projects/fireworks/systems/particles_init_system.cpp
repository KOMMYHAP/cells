#include "generated/auto_emit_new_particle_system.h"

#include "components/generated/auto_emitter.h"
#include "components/generated/auto_particle_init_request.h"
#include "game_config.h"

void EmitNewParticleSystem::DoProcessComponents(EcsEntity id, const Emitter& emitter)
{
    _ecsWorld->remove<NewParticleTag>(id);
    if (emitter.generation < 0 || emitter.generation >= _gameConfig->fireworks.size()) {
        ASSERT(false, "invalid emitter's generation!");
        return;
    }
    const GameConfig::FireworksConfig& fireworksConfig = _gameConfig->fireworks[emitter.generation];


}