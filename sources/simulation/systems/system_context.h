#pragma once

struct SystemContext {
    CellId id;
    const std::vector<std::byte*>& components;
};
