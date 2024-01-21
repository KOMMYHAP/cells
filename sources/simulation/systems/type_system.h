#pragma once

#include "brain/brain_packed_data.h"
#include "brain/cell.h"

class TypeSystem {
public:
    TypeSystem(uint32_t capacity);

    void Set(CellId id, CellType type);
    CellType Get(CellId id) const;

private:
    std::vector<CellType> _types;
};