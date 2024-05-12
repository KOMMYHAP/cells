#include "component_storage.h"

ComponentStorage::ComponentStorage(const Component& metaInfo, size_t count)
    : _info(metaInfo)
    , _buffer(count * _info.sizeInBytes)
{
    ASSUME(_info.sizeInBytes > 0);
}

std::byte& ComponentStorage::ModifyUnsafe(size_t componentIndex)
{
    const size_t bufferIndex = _info.sizeInBytes * componentIndex;
    ASSUME(bufferIndex < _buffer.size());
    return _buffer[bufferIndex];
}

const std::byte& ComponentStorage::GetUnsafe(size_t componentIndex) const
{
    const size_t bufferIndex = _info.sizeInBytes * componentIndex;
    ASSUME(bufferIndex < _buffer.size());
    return _buffer[bufferIndex];
}

size_t ComponentStorage::Count() const
{
    ASSUME(_buffer.size() % _info.sizeInBytes == 0);
    return _buffer.size() / _info.sizeInBytes;
}

size_t ComponentStorage::CountBytes() const
{
    return _buffer.size();
}
