#include "components/generated/auto_emitter.h"
#include "game_controller.h"
#include "generated/auto_emitters_update_stats_system.h"

void EmittersUpdateStatsSystem::DoSystemUpdate()
{
    const int32_t particlesCount = static_cast<int32_t>(_ecsWorld->view<Emitter>().size());
    _gameController->SetEmittersCount(particlesCount);
}