#include "generated/auto_empty_cell_draw_system.h"

#include "components/generated/auto_cell_position.h"
#include "game_config.h"
#include "widgets/world/world_rasterization_target.h"

void EmptyCellDrawSystem::DoProcessComponents(EcsEntity /*id*/, const CellPosition& cellPosition)
{
    _worldRasterizationTarget->Set(cellPosition, _gameConfig->emptyCellColor);
}