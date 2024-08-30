#include "keep_population_system.h"

KeepPopulationSystem::KeepPopulationSystem(const Config& config)
    : _config(config)
{
}

void KeepPopulationSystem::DoSystemUpdate()
{
    static constexpr size_t TargetCellsCount = { 10 };
    const size_t expectedCellsCount = _config.stats->GetCellsCount() + _config.stats->GetSpawnPlacesCount();
    if (expectedCellsCount >= TargetCellsCount) {
        return;
    }

    const size_t spawnPlaceToCreate = TargetCellsCount - expectedCellsCount;
    std::array<CellPosition, TargetCellsCount> spawnPlaces;

    Random::Accessor random { *_config.random };
    const uint32_t width = _config.locator->GetWidth();
    const uint32_t height = _config.locator->GetHeight();

    for (size_t i = 0; i < spawnPlaceToCreate; ++i) {
        const uint32_t cellsIndex = random.GetValue(0, NarrowCast<uint32_t>(_config.stats->GetCellsCapacity()));
        const auto x = NarrowCast<int16_t>(cellsIndex / width);
        const auto y = NarrowCast<int16_t>(cellsIndex % height);
        spawnPlaces[i] = CellPosition { x, y };
    }

    std::ranges::sort(spawnPlaces);
    const auto uniqueSpawnPlaces = std::ranges::unique(spawnPlaces);
    std::ranges::for_each(uniqueSpawnPlaces, [this](CellPosition position) {
        Spawn(position);
    });
}

void KeepPopulationSystem::Spawn(CellPosition position)
{
    const CellId id = _config.spawner->ScheduleSpawn(position);
    CellBrain& brain = _config.world->emplace<CellBrain>(id);
    _config.factory->Make(brain);
}