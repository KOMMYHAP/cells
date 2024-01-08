#pragma once
#include <cassert>

#include "cell.h"

enum class CellType : uint8_t {
    Unit,
    Food,
    Wall,
    Dummy
};

#pragma pack(push, 1)
struct BrainInfo {
    CellType type;
    sf::Vector2<uint16_t> position;
};
#pragma pack(pop)


class BrainView {
public:
    BrainView(const Cell& cell);

    const BrainInfo& GetBrainInfo() const;

    CellType GetType() const { return GetBrainInfo().type; }
    sf::Vector2<uint16_t> GetPosition() const { return GetBrainInfo().position; }

private:
    const Cell& _cell;
};
