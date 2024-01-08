#include "cell_search_proxy.h"
#include "world.h"
#include "Quadtree.h"

namespace {

// todo:
// 1. Profile this, remove extra allocations
// 2. Try this: https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det

using QuadTreeUnit = uint32_t;
using QuadTreeBox = quadtree::Box<QuadTreeUnit>;

static QuadTreeBox CellPositionToBox(const sf::Vector2u& position)
{
    return QuadTreeBox { position.x, position.y, 1, 1 };
}

struct CellBoxProvider {
    const World& world;

    QuadTreeBox operator()(const CellId id) const
    {
        const Cell& cell = world.Get(id);
        assert(cell.type != Type::Dummy);
        return CellPositionToBox(cell.position);
    }
};

using QuadTree = quadtree::Quadtree<CellId, CellBoxProvider, std::equal_to<CellId>, QuadTreeUnit>;
}

CellSearchProxy::CellSearchProxy(World& world, uint32_t width, uint32_t height)
    : _world(world)
{
    auto boxProvider = CellBoxProvider { _world };
    static_assert(sizeof(QuadTree) == _quadTreeMemorySize);
    static_assert(alignof(QuadTree) == _quadTreeAlignment);
    new (_quadtreeMemory) QuadTree(QuadTreeBox { 0, 0, width, height }, boxProvider);
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
