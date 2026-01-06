#include "conway_game_system.h"

#include "components/generated/auto_cell_position.h"
#include "widgets/world/world_rasterization_target.h"

ConwayGameSystem::ConwayGameSystem(ConwayGame& game, WorldRasterizationTarget& rasterizationTarget, SimulationPlayer& simulationPlayer)
    : _simulationPlayer(&simulationPlayer)
    , _game(&game)
    , _rasterizationTarget(&rasterizationTarget)
{
}

void ConwayGameSystem::Restart()
{
    _inProgress = true;
    _game->Restart();
    _simulationPlayer->Resume();
}

void ConwayGameSystem::DoSystemUpdate()
{
    if (_inProgress) {
        _summary = _game->DoNextGeneration();
    }

    _inProgress = _summary.state == ConwayGenerationState::Good;
    const Field& field = _game->GetCurrentGeneration();

    _rasterizationTarget->Lock();
    for (int32_t y = 0; y < field.GetSizeY(); ++y) {
        for (int32_t x = 0; x < field.GetSizeX(); ++x) {
            const FieldPosition position { x, y };
            const SDL_Color color = field.Has(position) ? SDL_Color { 255, 0, 0, 255 } : SDL_Color { 0, 0, 0, 255 };
            _rasterizationTarget->Set(CellPosition { static_cast<int16_t>(x), static_cast<int16_t>(y) }, color);
        }
    }
    _rasterizationTarget->Unlock();

    if (!_inProgress) {
        _simulationPlayer->Pause(); //< pause player when game finished
    }
}

ConwayGame* ConwayGameSystem::ModifyGame()
{
    if (_inProgress) {
        return nullptr;
    }
    return _game;
}