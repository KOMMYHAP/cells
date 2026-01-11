#include "game_controller.h"
#include "generated/auto_game_controller_update_system.h"
#include "world_statistics.h"

void GameControllerUpdateSystem::DoSystemUpdate()
{
    const Common::Time time = _worldStatistics->GetFrameTime();
    _gameController->SetFrameTime(time);
}