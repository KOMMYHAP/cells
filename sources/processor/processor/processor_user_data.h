#pragma once

inline constexpr size_t ProcessorUserDataSize = 8;

template <class T>
concept ProcessorUserDataType = std::is_trivial_v<T> && sizeof(T) <= ProcessorUserDataSize;

class ProcessorUserData {
public:
    ProcessorUserData() = default;

    template <ProcessorUserDataType T>
    explicit ProcessorUserData(const T value)
#if defined(COMMON_ASSERTS_ENABLED)
        : _valid(true)
#endif
    {
        memcpy(_data.data(), &value, sizeof(T));
    }

    template <ProcessorUserDataType T>
    T Get() const
    {
        ASSERT(_valid, "Invalid flow");
        T value;
        memcpy(&value, _data.data(), sizeof(T));
        return value;
    }

private:
    std::array<std::byte, ProcessorUserDataSize> _data;

#if defined(COMMON_ASSERTS_ENABLED)
    bool _valid { false };
#endif
};