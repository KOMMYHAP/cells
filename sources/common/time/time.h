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
    explicit Time(uint64_t us);

    uint64_t _microseconds { 0 };
};

bool operator==(Time left, Time right);
bool operator!=(Time left, Time right);

bool operator<(Time left, Time right);
bool operator>(Time left, Time right);

bool operator<=(Time left, Time right);
bool operator>=(Time left, Time right);

Time operator-(Time right) = delete;

Time operator+(Time left, Time right);
Time& operator+=(Time& left, Time right);

Time operator-(Time left, Time right);
Time& operator-=(Time& left, Time right);

Time operator*(Time left, float right);
Time operator*(float left, Time right);
Time& operator*=(Time& left, float right);

Time operator*(Time left, uint64_t right) = delete;
Time operator*(uint64_t left, Time right) = delete;
Time& operator*=(Time& left, uint64_t right) = delete;

Time operator/(Time left, float right);
Time& operator/=(Time& left, float right);
float operator/(Time left, Time right);

Time operator/(Time left, uint64_t right) = delete;
Time& operator/=(Time& left, uint64_t right) = delete;

Time operator%(Time left, Time right);
Time& operator%=(Time& left, Time right);

}