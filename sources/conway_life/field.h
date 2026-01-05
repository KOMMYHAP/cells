#pragma once
#include "field_position.h"

class Field {
    friend struct FieldHasher;
    friend struct FieldComparator;

public:
    Field(int32_t sizeX, int32_t sizeY);

    int32_t GetSizeX() const { return _sizeX; }
    int32_t GetSizeY() const { return _sizeY; }

    void Set(FieldPosition position, bool value);
    bool Has(FieldPosition position) const;

    int32_t GetNeighboursCount(FieldPosition position) const;

private:
    int32_t GetCellIndex(FieldPosition position) const;

    std::vector<bool> _cells;
    int32_t _sizeX { 0 };
    int32_t _sizeY { 0 };
    int32_t _blocksInRow {0};
};
