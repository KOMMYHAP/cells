#pragma once
#include "field.h"
#include "field_utilities.h"

enum class ConwayGenerationState : uint8_t {
    Good,
    NoCellsLeft,
    SameField,
};

struct ConwayGameSummary {
    ConwayGenerationState state { ConwayGenerationState::Good };
    int32_t aliveCells { 0 };
    std::optional<int32_t> similarGenerationNumber;
};

class ConwayGame {
public:
    ConwayGame(Field&& field);

    void Restart();
    ConwayGameSummary DoNextGeneration();

    int32_t GetCurrentGenerationNumber() const { return _currentGenerationNumber; }
    const Field& GetCurrentGeneration() const { return _currentField; }
    Field& ModifyCurrentGeneration() { return _currentField; }

private:
    struct FieldDescription {
        int32_t generationNumber { 0 };
    };

    int32_t _currentGenerationNumber { 1 };
    Field _currentField;
    Field _nextField;
    std::unordered_map<Field, FieldDescription, FieldHasher, FieldComparator> _previousGenerations;
};