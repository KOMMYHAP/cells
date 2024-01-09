#include "cell_search_proxy.h"
#include "Quadtree.h"
#include "brain.h"
#include "field.h"

namespace {

// todo:
// 1. Profile this, remove extra allocations
// 2. Try this: https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det

using QuadTreeUnit = uint16_t;
using QuadTreeBox = quadtree::Box<QuadTreeUnit>;

constexpr QuadTreeUnit quadTreeBoxCellSize = 1;

template <class T>
static QuadTreeBox CellPositionToBox(const sf::Vector2<T>& position)
{
    return QuadTreeBox { static_cast<QuadTreeUnit>(position.x), static_cast<QuadTreeUnit>(position.y), quadTreeBoxCellSize, quadTreeBoxCellSize };
}

struct CellBoxProvider {
    const Field& world;

    QuadTreeBox operator()(const CellId id) const
    {
        const Cell& cell = world.Get(id);
        return CellPositionToBox(ConstBrain(cell).GetInfo().position);
    }
};

using QuadTree = quadtree::Quadtree<CellId, CellBoxProvider, std::equal_to<CellId>, QuadTreeUnit>;
}

CellSearchProxy::CellSearchProxy(Field& world, uint32_t width, uint32_t height)
    : _world(world)
{
    auto boxProvider = CellBoxProvider { _world };
    static_assert(sizeof(QuadTree) == _quadTreeMemorySize);
    static_assert(alignof(QuadTree) == _quadTreeAlignment);

    const auto boxWidth = static_cast<QuadTreeUnit>(std::bit_ceil(width));
    const auto boxHeight = static_cast<QuadTreeUnit>(std::bit_ceil(height));
    new (_quadtreeMemory) QuadTree(QuadTreeBox { 0, 0, boxWidth, boxHeight }, boxProvider);
}

CellSearchProxy::~CellSearchProxy()
{
    As<QuadTree>().~QuadTree();
}

std::vector<CellId> CellSearchProxy::Find(const sf::Vector2u& position) const
{
    const auto box = CellPositionToBox(position);
    std::vector<CellId> ids = As<QuadTree>().query(box);
    return ids;
}

void CellSearchProxy::Add(CellId id)
{
    As<QuadTree>().add(id);
}

void CellSearchProxy::Remove(CellId id)
{
    As<QuadTree>().remove(id);
}
