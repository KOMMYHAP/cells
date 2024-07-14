#pragma once

inline constexpr size_t ProcessorUserDataSize = 8;

template <class T>
concept ProcessorUserDataType = std::is_trivial_v<T> && sizeof(T) <= ProcessorUserDataSize;

class ProcessorUserData {
public:
    ProcessorUserData() = default;

    template <ProcessorUserDataType T>
    explicit ProcessorUserData(const T value)
        : _valid(true)
    {
        memcpy(_data.data(), &value, sizeof(T));
    }

    template <ProcessorUserDataType T>
    T Get() const
    {
        ASSERT(_valid);
        T value;
        memcpy(&value, _data.data(), sizeof(T));
        return value;
    }

private:
    std::array<std::byte, ProcessorUserDataSize> _data;
    bool _valid { false };
};