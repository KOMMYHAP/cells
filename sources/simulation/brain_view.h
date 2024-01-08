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
    sf::Vector2<uint16_t> position;
};

struct BrainData {
    static constexpr uint32_t memorySize = Cell::brainSize - sizeof(BrainInfo);
    std::span<std::byte> memory;
};

class BrainView {
public:
    BrainView(const Cell& cell);

    const BrainInfo& GetBrainInfo() const;

    CellType GetType() const { return GetBrainInfo().type; }
    sf::Vector2<uint16_t> GetPosition() const { return GetBrainInfo().position; }

private:
    const Cell& _cell;
};
