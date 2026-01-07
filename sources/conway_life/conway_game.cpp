#include "conway_game.h"

#include "components/generated/auto_field_position.h"

void ConwayGameController::Restart()
{
    _previousGenerations.clear();
    _currentGenerationSummary = {};
    _currentGenerationSummary.state = ConwayGenerationState::Good;
}

void ConwayGameController::SetCurrentGeneration(Field&& field, int32_t aliveCells)
{
    _currentGenerationSummary.generationNumber += 1;
    _currentGenerationSummary.aliveCells = aliveCells;

    auto [similarFieldIt, inserted] = _previousGenerations.emplace(std::move(field), FieldDescription { _currentGenerationSummary.generationNumber });
    if (inserted) {
        _currentGenerationSummary.state = aliveCells > 0 ? ConwayGenerationState::Good : ConwayGenerationState::NoCellsLeft;
    } else {
        _currentGenerationSummary.state = ConwayGenerationState::SameField;
        _currentGenerationSummary.similarGenerationNumber = similarFieldIt->second.generationNumber;
    }
}
