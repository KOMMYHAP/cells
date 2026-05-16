#include "generated/auto_create_birth_emitter_system.h"

#include "sdl_utils.h"
#include "components/generated/auto_cell_emitter.h"
#include "components/generated/auto_cell_particle.h"
#include "game_config.h"

void CreateBirthEmitterSystem::DoProcessComponents(EcsEntity id)
{
    CellEmitter emitter{};
    emitter.particleLifetime = _gameConfig->birthEmitterLifetime;
    emitter.emitterLifetime = emitter.particleLifetime;
    emitter.colorFrom = PackColor(_gameConfig->emptyCellColor);
    emitter.colorTo = PackColor(_gameConfig->aliveCellColor);
    _ecsWorld->emplace_or_replace<CellEmitter>(id, emitter);
    _ecsWorld->remove<CellParticle>(id);
}