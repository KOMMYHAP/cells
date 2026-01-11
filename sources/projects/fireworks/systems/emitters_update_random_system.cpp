#include "components/generated/auto_emitter_random.h"
#include "game_controller.h"
#include "generated/auto_emitters_update_random_system.h"

void EmittersUpdateRandomSystem::DoProcessComponents(EcsEntity /*id*/, EmitterRandom& emitterRandom)
{
    emitterRandom.value = _gameController->ModifyRandom().GetValue();
}