#include "status_panel.h"
#include "simulation/simulation_statistics_provider.h"
#include "world.h"

constexpr uint16_t StatusMessageBufferLimit = 200;
constexpr uint16_t StatusTextSize = 10;
constexpr uint16_t StatusTextOffset = 0;

static auto GetTimeInfo(sf::Time time)
{
    const std::string_view tickUnit = time.asMilliseconds() >= 1000
        ? "s"
        : time.asMicroseconds() >= 1000
        ? "ms"
        : "us";
    const float tickTimeValue = time.asMilliseconds() >= 1000
        ? time.asSeconds()
        : time.asMicroseconds() >= 1000
        ? static_cast<float>(time.asMilliseconds())
        : static_cast<float>(time.asMicroseconds());

    return std::make_tuple(tickTimeValue, tickUnit);
}

StatusPanel::StatusPanel(const UiLayout& layout, const sf::Font& font, World& world)
    : _world(world)
{
    _text.setFont(font);
    _text.setCharacterSize(StatusTextSize);
    _text.setPosition(NarrowCast<float>(layout.fieldOffset + StatusTextSize), StatusTextOffset);

    _buffer.reserve(StatusMessageBufferLimit);
}

void StatusPanel::Update(sf::Time elapsedTime)
{
    _frameTimeCounter.AddSample(elapsedTime.asSeconds());
    sf::Time frameTime = sf::seconds(_frameTimeCounter.CalcMedian());

    const auto [frameTimeValue, frameUnit] = GetTimeInfo(frameTime);
    const auto fps = static_cast<uint16_t>(1.0f / frameTime.asSeconds());

    const SimulationStatisticsProvider& statistics = _world.GetSimulationStatistics();
    const size_t cellsCount = statistics.GetCellsCount();
    const float cellsCountPercent = static_cast<uint8_t>(static_cast<float>(cellsCount) / (statistics.GetCellsCapacity()) * 100.0f);

    _buffer.clear();
    std::format_to_n(std::back_inserter(_buffer), _buffer.capacity(),
        "FPS {:4} | Frame {:4}{:2} | Cells {:8} ({:2}%) | Spawns {:3} | Death Age {:3} | Death Energy {:3}",
        fps,
        frameTimeValue, frameUnit,
        cellsCount, cellsCountPercent,
        statistics.GetSpawnPlacesCount(),
        statistics.GetDeathFromAge(),
        statistics.GetDeathFromEmptyEnergy());
    
    _text.setString(sf::String(_buffer));
}

void StatusPanel::Draw(sf::RenderTarget& target)
{
    target.draw(_text);
}