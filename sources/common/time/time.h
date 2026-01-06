#pragma once

namespace Common {

class Time {
public:
    Time() = default;

    static Time FromSeconds(double seconds);
    static Time FromMilliseconds(int64_t ms);
    static Time FromMicroseconds(int64_t us);

    double AsSeconds() const;
    int64_t AsMilliseconds() const;
    int64_t AsMicroseconds() const;
    bool IsZero() const;

private:
    explicit Time(int64_t us);

    int64_t _microseconds { 0 };
};

bool operator==(Time left, Time right);
bool operator!=(Time left, Time right);

bool operator<(Time left, Time right);
bool operator>(Time left, Time right);

bool operator<=(Time left, Time right);
bool operator>=(Time left, Time right);

Time operator-(Time right);

Time operator+(Time left, Time right);
Time& operator+=(Time& left, Time right);

Time operator-(Time left, Time right);
Time& operator-=(Time& left, Time right);

Time operator*(Time left, double right);
Time operator*(double left, Time right);
Time& operator*=(Time& left, double right);

Time operator*(Time left, uint64_t right) = delete;
Time operator*(uint64_t left, Time right) = delete;
Time& operator*=(Time& left, uint64_t right) = delete;

Time operator/(Time left, double right);
Time& operator/=(Time& left, double right);
double operator/(Time left, Time right);

Time operator/(Time left, uint64_t right) = delete;
Time& operator/=(Time& left, uint64_t right) = delete;

Time operator%(Time left, Time right);
Time& operator%=(Time& left, Time right);

}