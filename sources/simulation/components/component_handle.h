#pragma once

struct ComponentHandle {
    uint16_t id { 0 };
};

inline bool operator<(const ComponentHandle& lhs, const ComponentHandle& rhs)
{
    return lhs.id < rhs.id;
}
