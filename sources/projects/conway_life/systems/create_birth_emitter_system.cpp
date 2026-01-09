#include "generated/auto_create_birth_emitter_system.h"

#include "components/generated/auto_cell_emitter.h"
#include "components/generated/auto_cell_particle.h"
#include "game_config.h"
#include "sdl_utils.h"

void CreateBirthEmitterSystem::DoProcessComponents(EcsEntity id)
{
    CellEmitter emitter{};
    emitter.particle_lifetime = _gameConfig->birthEmitterLifetime;
    emitter.emitter_lifetime = emitter.particle_lifetime;
    emitter.color_from = PackColor(_gameConfig->emptyCellColor);
    emitter.color_to = PackColor(_gameConfig->aliveCellColor);
    _ecsWorld->emplace_or_replace<CellEmitter>(id, emitter);
    _ecsWorld->remove<CellParticle>(id);
}