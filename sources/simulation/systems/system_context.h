#pragma once

#include "components/cell_id.h"

struct SystemContext {
    CellId id;
    std::span<std::byte*> components;
};
