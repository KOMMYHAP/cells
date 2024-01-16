#include "field_search_proxy.h"
#include "brain/brain.h"
#include "field.h"
#include "field/quadtree/Quadtree.h"
#include "simulation_profile_category.h"

namespace {

// todo:
// 1. Profile this, remove extra allocations
// 2. Try this: https://stackoverflow.com/questions/41946007/efficient-and-well-explained-implementation-of-a-quadtree-for-2d-collision-det

using QuadTreeUnit = uint16_t;
using QuadTreeBox = quadtree::Box<QuadTreeUnit>;

template <class T>
static QuadTreeBox CellPositionToBox(const sf::Vector2<T>& position, QuadTreeUnit cellSize)
{
    return QuadTreeBox { static_cast<QuadTreeUnit>(position.x), static_cast<QuadTreeUnit>(position.y), cellSize, cellSize };
}

struct CellBoxProvider final {
    const Field& world;
    const QuadTreeUnit cellSize;

    QuadTreeBox operator()(const CellId id) const
    {
        const Cell& cell = world.Get(id);
        return CellPositionToBox(ConstBrain(cell).GetInfo().position, cellSize);
    }
};

using QuadTree = quadtree::Quadtree<CellId, CellBoxProvider, std::equal_to<CellId>, QuadTreeUnit>;
}

FieldSearchProxy::FieldSearchProxy(Field& world, uint32_t width, uint32_t height, uint32_t bufferSize, uint32_t cellSize)
    : _world(world)
    , _searchBuffer(bufferSize)
    , _cellSize(cellSize)
{
    auto boxProvider = CellBoxProvider { _world, static_cast<QuadTreeUnit>(_cellSize) };
    static_assert(sizeof(QuadTree) == _quadTreeMemorySize);
    static_assert(alignof(QuadTree) == _quadTreeAlignment);

    const auto boxWidth = static_cast<QuadTreeUnit>(std::bit_ceil(width));
    const auto boxHeight = static_cast<QuadTreeUnit>(std::bit_ceil(height));
    new (_quadtreeMemory) QuadTree(QuadTreeBox { 0, 0, boxWidth, boxHeight }, boxProvider);
}

FieldSearchProxy::~FieldSearchProxy()
{
    As<QuadTree>().~QuadTree();
}

std::span<const CellId> FieldSearchProxy::Find(const sf::Vector2u& position, uint32_t searchSizeLimit) const
{
    common::ProfileScope searchProxyProfileScope { "SearchProxy::Find", SimulationProfileCategory };

    const auto box = CellPositionToBox(position, static_cast<QuadTreeUnit>(_cellSize));

    std::span searchBuffer { _searchBuffer };
    if (searchSizeLimit < searchBuffer.size()) {
        searchBuffer = searchBuffer.subspan(searchSizeLimit);
    }
    return As<QuadTree>().query(box, searchBuffer);
}

void FieldSearchProxy::Add(CellId id)
{
    common::ProfileScope searchProxyProfileScope { "SearchProxy::Add", SimulationProfileCategory };
    As<QuadTree>().add(id);
}

void FieldSearchProxy::Remove(CellId id)
{
    common::ProfileScope searchProxyProfileScope { "SearchProxy::Remove", SimulationProfileCategory };
    As<QuadTree>().remove(id);
}
