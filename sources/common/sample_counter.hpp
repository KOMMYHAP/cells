
namespace common {

template <class Sample, uint16_t SamplesLimit>
    requires std::is_arithmetic_v<Sample>
void SampleCounter<Sample, SamplesLimit>::Reset()
{
    _currentSample = 0;
    _availableSamplesCount = 0;
    _samples.fill(Sample {});
}

template <class Sample, uint16_t SamplesLimit>
    requires std::is_arithmetic_v<Sample>
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
    requires std::is_arithmetic_v<Sample>
Sample SampleCounter<Sample, SamplesLimit>::CalcMedian() const
{
    if (IsEmpty()) {
        return {};
    }

    auto samples = _samples;
    std::sort(samples.begin(), samples.begin() + _availableSamplesCount);

    if (_availableSamplesCount % 2 == 0) {
        return static_cast<Sample>(static_cast<float>(samples[_availableSamplesCount / 2 - 1] + samples[_availableSamplesCount / 2]) / 2.0f);
    }

    return samples[_availableSamplesCount / 2];
}

template <class Sample, uint16_t SamplesLimit>
    requires std::is_arithmetic_v<Sample>
Sample SampleCounter<Sample, SamplesLimit>::CalcAverage() const
{
    if (IsEmpty()) {
        return {};
    }

    const Sample total = std::accumulate(_samples.begin(), _samples.begin() + _availableSamplesCount, Sample {});
    return static_cast<Sample>(static_cast<float>(total) / _availableSamplesCount);
}

}