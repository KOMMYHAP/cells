#pragma once
#include "field.h"
#include "field_utilities.h"

enum class ConwayGenerationState : uint8_t {
    Good,
    NoCellsLeft,
    SameField,
};

struct ConwayGameSummary {
    ConwayGenerationState state { ConwayGenerationState::NoCellsLeft };
    int32_t aliveCells { 0 };
    std::optional<int32_t> similarGenerationNumber;
    const Field* currentGeneration { nullptr };
    int32_t generationNumber { 1 };
};

class ConwayGameController {
public:
    void Restart();
    void SetCurrentGeneration(Field&& field, int32_t aliveCells);
    ConwayGameSummary GetSummary() const { return _currentGenerationSummary; }

private:
    struct FieldDescription {
        int32_t generationNumber { 0 };
    };

    ConwayGameSummary _currentGenerationSummary;
    std::unordered_map<Field, FieldDescription, FieldHasher, FieldComparator> _previousGenerations;
};
