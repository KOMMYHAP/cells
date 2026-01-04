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

    for (int32_t x = 0; x < _currentField.GetSizeX(); ++x) {
        for (int32_t y = 0; y < _currentField.GetSizeY(); ++y) {
            const FieldPosition position { x, y };
            const int32_t neighboursCount = _currentField.GetNeighboursCount(position);
            const bool willAlive = neighboursCount == 2 || neighboursCount == 3;

            summary.aliveCells += willAlive ? 1 : 0;
            _nextField.Set(position, willAlive);
        }
    }
    _currentField = _nextField;

    auto [similarFieldIt, inserted] = _previousGenerations.emplace(_nextField, FieldDescription { _currentGenerationNumber });
    if (inserted) {
        summary.state = summary.aliveCells > 0 ? ConwayGenerationState::Good : ConwayGenerationState::NoCellsLeft;
    } else {
        summary.state = ConwayGenerationState::SameField;
        summary.similarGenerationNumber = similarFieldIt->second.generationNumber;
    }

    _currentGenerationNumber += 1;
    return summary;
}