#pragma once

namespace Common {

class Time {
public:
    static Time FromSeconds(float seconds);
    static Time FromMilliseconds(uint64_t ms);
    static Time FromMicroseconds(uint64_t us);

    float AsSeconds() const;
    uint64_t AsMilliseconds() const;
    uint64_t AsMicroseconds() const;

    static const inline Time Zero { 0 };

private:
    explicit Time(uint64_t microseconds);

    uint64_t _microseconds { 0 };
};

}