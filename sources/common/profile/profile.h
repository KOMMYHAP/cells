#pragma once

#include "profile_config.h"

namespace common {

enum class ProfileCategory : uint8_t {
    INVALID = std::numeric_limits<uint8_t>::max()
};

namespace details {
    void ProfileBeginEvent(std::string_view name, ProfileCategory category);
    void ProfileEndEvent();
}

ProfileCategory MakeProfileCategory();

inline void ProfileBeginEvent([[maybe_unused]] std::string_view name, [[maybe_unused]] ProfileCategory category)
{
    if constexpr (ProfileEnabled) {
        details::ProfileBeginEvent(name, category);
    }
}

inline void ProfileEndEvent()
{
    if constexpr (ProfileEnabled) {
        details::ProfileEndEvent();
    }
}

struct ProfileScope final {
    ProfileScope(std::string_view name, ProfileCategory category)
    {
        ProfileBeginEvent(name, category);
    }

    ~ProfileScope()
    {
        ProfileEndEvent();
    }
};

}