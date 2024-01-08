#pragma once

class Brain;
class BrainView;

enum class CellId : uint32_t {
    Invalid = std::numeric_limits<uint32_t>::max()
};

constexpr auto CellIdToInt(CellId id) { return static_cast<std::underlying_type_t<CellId>>(id); }

struct Cell {
    static constexpr uint32_t brainSize = 64;
    std::byte brain[brainSize];

    Brain GetBrain();
    BrainView GetBrain() const;
};
