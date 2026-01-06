#pragma once
#include "sample_counter.h"

namespace Common {

template <class Sample, uint16_t SamplesLimit>
void SampleCounter<Sample, SamplesLimit>::Reset()
{
    _currentSample = 0;
    _availableSamplesCount = 0;
    _samples.fill(Sample {});
}

template <class Sample, uint16_t SamplesLimit>
void SampleCounter<Sample, SamplesLimit>::AddSample(Sample sample)
{
    _samples[_currentSample] = sample;
    _currentSample += 1;
    if (_currentSample == SamplesLimit) {
        _currentSample = 0;
    }
    _availableSamplesCount = std::min<uint16_t>(_availableSamplesCount + 1, SamplesLimit);
}

template <class Sample, uint16_t SamplesLimit>
Sample SampleCounter<Sample, SamplesLimit>::CalcMedian() const
{
    if (IsEmpty()) {
        return {};
    }

    auto samples = _samples;
    std::sort(samples.begin(), samples.begin() + _availableSamplesCount);

    // Take less value if samples count is even
    const uint16_t indexOffset = (_availableSamplesCount % 2 != 0) ? 0 : 1;
    return samples[_availableSamplesCount / 2 - indexOffset];
}

template <class Sample, uint16_t SamplesLimit>
Sample SampleCounter<Sample, SamplesLimit>::CalcAverage() const
{
    if (IsEmpty()) {
        return {};
    }

    const Sample total = std::accumulate(_samples.begin(), _samples.begin() + _availableSamplesCount, Sample {});
    return total / static_cast<double>(_availableSamplesCount);
}

}