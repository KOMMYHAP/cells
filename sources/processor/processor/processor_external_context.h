#pragma once

class ProcessorExternalContext {
public:
    explicit ProcessorExternalContext(std::any data)
        : _data(std::move(data))
    {
    }

    template <class T>
    T& Modify()
    {
        T* value = std::any_cast<T>(&_data);
        ASSERT(value != nullptr);
        return *value;
    }

    template <class T>
    const T& Get() const
    {
        const T* value = std::any_cast<T>(&_data);
        ASSERT(value != nullptr);
        return *value;
    }

private:
    std::any _data;
};