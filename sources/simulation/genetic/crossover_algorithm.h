#pragma once

#include "components/cell_brain.h"

class CrossoverAlgorithm {
public:
    CrossoverAlgorithm(uint8_t kPoints);

    CellBrain Combine(const CellBrain& parentLeft, const CellBrain& parentRight);

private:
    uint8_t _kPoints { 0 };
};