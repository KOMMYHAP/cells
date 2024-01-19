#include "brain/cell.h"

class Field;

class QuadTreeCellBox {
public:
    uint16_t left { 0 };
    uint16_t top { 0 };
    uint16_t width { 0 };
    uint16_t height { 0 };

    uint16_t getRight() const noexcept
    {
        return left + width;
    }

    uint16_t getBottom() const noexcept
    {
        return top + height;
    }

    sf::Vector2<uint16_t> getTopLeft() const noexcept
    {
        return sf::Vector2<uint16_t>(left, top);
    }

    sf::Vector2<uint16_t> getCenter() const noexcept
    {
        return sf::Vector2<uint16_t>(left + width / 2, top + height / 2);
    }

    sf::Vector2<uint16_t> getSize() const noexcept
    {
        return sf::Vector2<uint16_t>(width, height);
    }

    bool contains(const QuadTreeCellBox& box) const noexcept
    {
        return left <= box.left && box.getRight() <= getRight() && top <= box.top && box.getBottom() <= getBottom();
    }

    bool intersects(const QuadTreeCellBox& box) const noexcept
    {
        return !(left >= box.getRight() || getRight() <= box.left || top >= box.getBottom() || getBottom() <= box.top);
    }
};

class QuadTreeCellBoxProvider {
private:
    QuadTreeCellBox GetBox(CellId id) const;

public:
    Field& field;
};


class QuadTree {
public:
    void Add(CellId id);
    void Remove(CellId id);
    CellId Find(const sf::Vector2<uint16_t>& position) const;

private:
    inline static constexpr uint32_t LeafMarker = std::numeric_limits<uint32_t>::max();
    struct Node {
        uint32_t firstChildIndex { LeafMarker };
        CellId cellIndex;
    };

    std::vector<uint32_t> _freeIndexes;

    const QuadTreeCellBoxProvider& _boxProvider;
};