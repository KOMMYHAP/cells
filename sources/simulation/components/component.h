#pragma once

struct Component {
    uint16_t id { 0 };
    uint16_t sizeInBytes { 0 };
};

inline bool operator<(const Component& lhs, const Component& rhs)
{
    return lhs.id < rhs.id;
}
