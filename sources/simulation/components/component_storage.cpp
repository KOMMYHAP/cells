#include "component_storage.h"

ComponentStorage::ComponentStorage(const Component& metaInfo, uint32_t count)
    : _info(metaInfo)
    , _buffer(count * _info.sizeInBytes)
{
    ASSUME(_info.sizeInBytes > 0);
}

std::byte& ComponentStorage::ModifyUnsafe(CellId id)
{
    ASSUME(id != CellId::Invalid);
    const uint32_t bufferIndex = _info.sizeInBytes * static_cast<uint32_t>(id);
    ASSUME(bufferIndex < _buffer.size());
    return _buffer[bufferIndex];
}

const std::byte& ComponentStorage::GetUnsafe(CellId id) const
{
    ASSUME(id != CellId::Invalid);
    const uint32_t bufferIndex = _info.sizeInBytes * static_cast<uint32_t>(id);
    ASSUME(bufferIndex < _buffer.size());
    return _buffer[bufferIndex];
}

uint32_t ComponentStorage::GetCellsCount() const
{
    ASSUME(_info.sizeInBytes > 0);
    ASSUME(_buffer.size() % _info.sizeInBytes == 0);
    return _buffer.size() / _info.sizeInBytes;
}