#include "components/generated/auto_cell_position.h"
#include "components/generated/auto_has_life_tag.h"
#include "conway_game.h"
#include "field.h"
#include "generated/auto_update_game_controller_system.h"
#include "simulation/cell_locator.h"

void UpdateGameControllerSystem::DoSystemUpdate()
{
    int32_t aliveCells = 0;
    Field field { _cellLocator->GetWidth(), _cellLocator->GetHeight() };
    _ecsWorld->view<HasLifeTag, const CellPosition>().each([&aliveCells, &field](const CellPosition& position) {
        aliveCells += 1;
        field.Set(position, true);
    });

    _conwayGameController->SetCurrentGeneration(std::move(field), aliveCells);
}