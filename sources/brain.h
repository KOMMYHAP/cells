#pragma once

#include "cell.h"

enum class CellType : uint8_t {
    Unit,
    Food,
    Wall,
    Dummy
};

struct BrainInfo {
    CellType type;
};

struct BrainData {
    static constexpr uint32_t memorySize = Cell::brainSize - sizeof(BrainInfo);
    std::span<std::byte> memory;
};

class Brain {
public:
    Brain(Cell& cell);

    CellType GetType() const;

    void Process();

private:
    BrainInfo& GetBrainInfo();
    const BrainInfo& GetBrainInfo() const;

    void ProcessUnit(const BrainData& memory);
    void ProcessFood(const BrainData& memory);
    void ProcessWall(const BrainData& memory);

    Cell& _cell;
};