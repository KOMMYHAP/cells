#include "component_storage.h"

ComponentStorage::ComponentStorage(const Component& metaInfo, uint32_t count)
    : _info(metaInfo)
    , _buffer(count * _info.sizeInBytes)
{
    ASSERT(_info.sizeInBytes > 0);
}

std::byte& ComponentStorage::ModifyUnsafe(CellId id)
{
    ASSERT(id != CellId::Invalid);
    const uint32_t bufferIndex = _info.sizeInBytes * static_cast<uint32_t>(id);
    ASSERT(bufferIndex < _buffer.size());
    return _buffer[bufferIndex];
}

const std::byte& ComponentStorage::GetUnsafe(CellId id) const
{
    ASSERT(id != CellId::Invalid);
    const uint32_t bufferIndex = _info.sizeInBytes * static_cast<uint32_t>(id);
    ASSERT(bufferIndex < _buffer.size());
    return _buffer[bufferIndex];
}

uint32_t ComponentStorage::GetCellsCount() const
{
    ASSERT(_info.sizeInBytes > 0);
    ASSERT(_buffer.size() % _info.sizeInBytes == 0);
    return NarrowCast<uint32_t>(_buffer.size()) / _info.sizeInBytes;
}