#include "time_counter.h"

namespace common {

void TimeCounter::AddSample(sf::Time elapsedTime)
{
    if (_samplesCount == SamplesCountLimit) {
        return;
    }

    _samples[_samplesCount] = elapsedTime.asSeconds();
    _samplesCount += 1;
}

void TimeCounter::Reset()
{
    _samplesCount = 0;
    _timePerTick = {};
}

sf::Time TimeCounter::GetAverageTime() const
{
    if (IsEmpty()) {
        return sf::Time::Zero;
    }

    const float totalTime = std::accumulate(_samples.begin(), _samples.begin() + _samplesCount, 0.0f);
    return sf::seconds(totalTime / _samplesCount);
}

sf::Time TimeCounter::GetMedianTime() const
{
    if (IsEmpty()) {
        return sf::Time::Zero;
    }

    auto samples = _samples;
    std::sort(samples.begin(), samples.begin() + _samplesCount);

    if (_samplesCount % 2 == 0) {
        return sf::seconds((samples[_samplesCount / 2 - 1] + samples[_samplesCount / 2]) / 2);
    }

    return sf::seconds(samples[_samplesCount / 2]);
}

}