#include "generated/auto_draw_empty_cell_system.h"

#include "game_config.h"
#include "widgets/world/world_rasterization_target.h"
#include "components/generated/auto_cell_position.h"

void DrawEmptyCellSystem::DoProcessComponents(EcsEntity /*id*/, const CellPosition& cellPosition)
{
    _worldRasterizationTarget->Set(cellPosition, _gameConfig->emptyCellColor);
}