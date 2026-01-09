#include "generated/auto_draw_alive_cell_system.h"

#include "components/generated/auto_cell_position.h"
#include "widgets/world/world_rasterization_target.h"
#include "game_config.h"

void DrawAliveCellSystem::DoProcessComponents(EcsEntity /*id*/, const CellPosition& cellPosition)
{
    _worldRasterizationTarget->Set(cellPosition, _gameConfig->aliveCellColor);
}