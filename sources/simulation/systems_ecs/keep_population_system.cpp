#include "systems_ecs/generated/auto_keep_population_system.h"

#include "cell_factories/random_cell_factory.h"
#include "components/generated/auto_cell_brain.h"
#include "simulation/cell_locator.h"
#include "simulation/simulation_statistics_provider.h"
#include "simulation/spawner.h"

void KeepPopulationSystem::DoSystemUpdate()
{
    static constexpr int64_t TargetCellsCount = { 10'000 };
    static constexpr int64_t SpawnPlacesBatchLimit = { 100 };

    const int64_t expectedCellsCount = _simulationStatisticsProvider->GetCellsCount() + _simulationStatisticsProvider->GetSpawnPlacesCount();
    if (expectedCellsCount >= TargetCellsCount) {
        return;
    }

    const int64_t spawnPlaceToCreate = std::min(TargetCellsCount - expectedCellsCount, SpawnPlacesBatchLimit);
    std::array<CellPosition, SpawnPlacesBatchLimit> spawnPlacesBuffer;

    Random::Accessor random { *_randomEngine };
    const uint32_t width = _cellLocator->GetWidth();

    for (int64_t i = 0; i < spawnPlaceToCreate; ++i) {
        const uint32_t cellsIndex = random.GetValue(0, NarrowCast<uint32_t>(_simulationStatisticsProvider->GetCellsCapacity() - 1));
        const auto x = NarrowCast<int16_t>(cellsIndex % width);
        const auto y = NarrowCast<int16_t>(cellsIndex / width);
        spawnPlacesBuffer[i] = CellPosition { x, y };
    }

    auto spawnPlaces = spawnPlacesBuffer | std::views::take(spawnPlaceToCreate);
    std::ranges::sort(spawnPlaces);
    const auto duplicates = std::ranges::unique(spawnPlaces);
    const auto uniqueSpawnPlaces = spawnPlaces | std::views::take(spawnPlaces.size() - duplicates.size());
    std::ranges::for_each(uniqueSpawnPlaces, [this](CellPosition position) {
        const EcsEntity id = _spawner->ScheduleSpawn(position);
        CellBrain& brain = _ecsWorld->emplace<CellBrain>(id);
        _randomCellFactory->Make(brain);
    });
}
