#include "conway_game.h"

#include "field_position.h"

ConwayGame::ConwayGame(Field&& field)
    : _currentField(std::move(field))
    , _nextField(_currentField)
{
}
void ConwayGame::Restart()
{
    _previousGenerations = {};
    _currentGenerationNumber = 1;
}

ConwayGameSummary ConwayGame::DoNextGeneration()
{
    ConwayGameSummary summary;

    for (int32_t y = 0; y < _currentField.GetSizeY(); ++y) {
        for (int32_t x = 0; x < _currentField.GetSizeX(); ++x) {
            const FieldPosition position { x, y };
            const bool hasLife = _currentField.Has(position);
            const int32_t neighboursCount = _currentField.GetNeighboursCount(position);

            if (!hasLife && neighboursCount == 3) {
                summary.aliveCells += 1;
                _nextField.Set(position, true);
            } else if (hasLife && (neighboursCount == 2 || neighboursCount == 3)) {
                summary.aliveCells += 1;
                _nextField.Set(position, true);
            } else {
                _nextField.Set(position, false);
            }
        }
    }
    _currentField = _nextField;

    auto [similarFieldIt, inserted] = _previousGenerations.emplace(_currentField, FieldDescription { _currentGenerationNumber });
    if (inserted) {
        summary.state = summary.aliveCells > 0 ? ConwayGenerationState::Good : ConwayGenerationState::NoCellsLeft;
    } else {
        summary.state = ConwayGenerationState::SameField;
        summary.similarGenerationNumber = similarFieldIt->second.generationNumber;
    }

    _currentGenerationNumber += 1;
    return summary;
}