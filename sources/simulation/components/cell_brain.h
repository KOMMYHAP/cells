#pragma once

inline static constexpr uint8_t CellBrainLimit = 255;

struct CellBrain {
    std::byte data[CellBrainLimit];
};
