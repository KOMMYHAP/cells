
namespace common {

template <class Sample, uint16_t SamplesLimit>
    requires std::is_arithmetic_v<Sample>
void SampleCounter<Sample, SamplesLimit>::Reset()
{
    _samplesCount = 0;
}

template <class Sample, uint16_t SamplesLimit>
    requires std::is_arithmetic_v<Sample>
void SampleCounter<Sample, SamplesLimit>::AddSample(Sample sample)
{
    if (IsFull()) {
        return;
    }

    _samples[_samplesCount] = sample;
    _samplesCount += 1;
}

template <class Sample, uint16_t SamplesLimit>
    requires std::is_arithmetic_v<Sample>
Sample SampleCounter<Sample, SamplesLimit>::CalcMedian() const
{
    if (IsEmpty()) {
        return {};
    }

    auto samples = _samples;
    std::sort(samples.begin(), samples.begin() + _samplesCount);

    if (_samplesCount % 2 == 0) {
        return (samples[_samplesCount / 2 - 1] + samples[_samplesCount / 2]) / 2;
    }

    return samples[_samplesCount / 2];
}

template <class Sample, uint16_t SamplesLimit>
    requires std::is_arithmetic_v<Sample>
Sample SampleCounter<Sample, SamplesLimit>::CalcAverage() const
{
    if (IsEmpty()) {
        return {};
    }

    const Sample total = std::accumulate(_samples.begin(), _samples.begin() + _samplesCount, Sample {});
    return total / _samplesCount;
}

}