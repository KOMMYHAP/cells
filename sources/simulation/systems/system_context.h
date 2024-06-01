#pragma once

#include "components/cell_id.h"

struct SystemContext {
    CellId id;
    const std::vector<std::byte*>& components;
};
