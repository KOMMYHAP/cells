#pragma once

namespace common {

template <class Sample, uint16_t SamplesLimit>
    requires std::is_arithmetic_v<Sample>
class SampleCounter {
public:
    void AddSample(Sample sample);
    void Reset();

    bool IsEmpty() const { return _availableSamplesCount == 0; }
    bool IsReady() const { return _availableSamplesCount > 0; }
    bool IsFull() const { return _currentSample == SamplesLimit; }

    Sample CalcAverage() const;
    Sample CalcMedian() const;

private:
    std::array<Sample, SamplesLimit> _samples;
    uint16_t _availableSamplesCount { 0 };
    uint16_t _currentSample { 0 };
};

}

#include "sample_counter.hpp"