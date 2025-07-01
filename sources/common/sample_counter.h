#pragma once

namespace Common {

template <class Sample, uint16_t SamplesLimit>
class SampleCounter {
public:
    void AddSample(Sample sample);
    void Reset();

    bool IsEmpty() const { return _availableSamplesCount == 0; }
    bool IsFull() const { return _availableSamplesCount == SamplesLimit; }

    Sample CalcAverage() const;
    Sample CalcMedian() const;

private:
    std::array<Sample, SamplesLimit> _samples {};
    uint16_t _availableSamplesCount { 0 };
    uint16_t _currentSample { 0 };
};

}

#include "sample_counter.hpp"