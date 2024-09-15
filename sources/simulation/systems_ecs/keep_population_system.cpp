#include "keep_population_system.h"

KeepPopulationSystem::KeepPopulationSystem(Config config)
    : _config(std::move(config))
{
}

void KeepPopulationSystem::DoSystemUpdate()
{
    static constexpr size_t TargetCellsCount = { 1'000 };
    static constexpr size_t SpawnPlacesBatchLimit = { 100 };

    const size_t expectedCellsCount = _config.stats->GetCellsCount() + _config.stats->GetSpawnPlacesCount();
    if (expectedCellsCount >= TargetCellsCount) {
        return;
    }

    const size_t spawnPlaceToCreate = std::min(TargetCellsCount - expectedCellsCount, SpawnPlacesBatchLimit);
    std::array<CellPosition, SpawnPlacesBatchLimit> spawnPlacesBuffer;

    Random::Accessor random { *_config.random };
    const uint32_t width = _config.locator->GetWidth();

    for (size_t i = 0; i < spawnPlaceToCreate; ++i) {
        const uint32_t cellsIndex = random.GetValue(0, NarrowCast<uint32_t>(_config.stats->GetCellsCapacity()));
        const auto x = NarrowCast<int16_t>(cellsIndex % width);
        const auto y = NarrowCast<int16_t>(cellsIndex / width);
        spawnPlacesBuffer[i] = CellPosition { x, y };
    }

    auto spawnPlaces = spawnPlacesBuffer | std::views::take(spawnPlaceToCreate);
    std::ranges::sort(spawnPlaces);
    const auto duplicates = std::ranges::unique(spawnPlaces);
    const auto uniqueSpawnPlaces = spawnPlaces | std::views::take(spawnPlaces.size() - duplicates.size());
    std::ranges::for_each(uniqueSpawnPlaces, [this](CellPosition position) {
        Spawn(position);
    });
}

void KeepPopulationSystem::Spawn(CellPosition position)
{
    const EcsEntity id = _config.spawner->ScheduleSpawn(position);
    CellBrain& brain = _config.world->emplace<CellBrain>(id);
    _config.factory->Make(brain);
}