#pragma once
#include "components/generated/auto_cell_position.h"

class Field {
    friend struct FieldHasher;
    friend struct FieldComparator;

public:
    Field(int32_t sizeX, int32_t sizeY);

    int32_t GetSizeX() const { return _sizeX; }
    int32_t GetSizeY() const { return _sizeY; }

    void Set(CellPosition position, bool value);
    bool Has(CellPosition position) const;

private:
    int32_t GetCellIndex(CellPosition position) const;

    std::vector<bool> _cells;
    int32_t _sizeX { 0 };
    int32_t _sizeY { 0 };
};
