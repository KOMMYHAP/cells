#pragma once

#include "components/cell_id.h"
#include "components/cell_type.h"

class TypeSystem {
public:
    TypeSystem(uint32_t capacity);

    void Set(CellId id, CellType type);
    CellType Get(CellId id) const;

private:
    std::vector<CellType> _types;
};