#pragma once

#include "components/generated/auto_cell_brain.h"

class CrossoverAlgorithm {
public:
    CrossoverAlgorithm(uint8_t point);
    CellBrain Combine(const CellBrain& parentLeft, const CellBrain& parentRight);

private:
    uint8_t _point { 0 };
};