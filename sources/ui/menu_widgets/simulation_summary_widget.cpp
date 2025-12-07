#include "simulation_summary_widget.h"

#include "simulation/simulation_statistics_provider.h"
#include "world.h"
#include "world_statistics.h"

SimulationSummaryWidget::SimulationSummaryWidget(World& world, const WorldStatistics& worldStats, const SimulationStatisticsProvider& stats)
    : _world(&world)
    , _worldStats(&worldStats)
    , _simulationStats(&stats)
{
}

BaseMenuWidget::MenuWidgetAction SimulationSummaryWidget::ProcessMenuItem(Common::Time /*time*/)
{
    constexpr auto ToString = [](World::Phase phase) {
        switch (phase) {
        case World::Phase::Stopped:
            return "stopped";
        case World::Phase::Running:
            return "running";
        case World::Phase::Paused:
            return "paused";
        }
        return "unknown";
    };
    const std::string_view phaseStr { ToString(_world->GetPhase()) };

    ImGui::Text("Simulation phase: %.*s", static_cast<int>(phaseStr.length()), phaseStr.data());
    ImGui::Text("Update: %" PRId64 " (%" PRId64 " ms)", _worldStats->GetElapsedFramesCount(), _worldStats->GetFrameTime().AsMilliseconds());
    ImGui::Text("Tick: %" PRId64 " (%" PRId64 " ms)", _worldStats->GetElapsedTicksCount(), _worldStats->GetTickTime().AsMilliseconds());
    ImGui::Text("Cells: %" PRId64 "/%" PRId64 " (alive/limit)", _simulationStats->GetCellsCount(), _simulationStats->GetCellsCapacity());
    ImGui::Text("Spawn places: %" PRId64, _simulationStats->GetSpawnPlacesCount());
    ImGui::Text("Death: energy %" PRId64 ", age %" PRId64, _simulationStats->GetDeathFromEmptyEnergy(), _simulationStats->GetDeathFromAge());

    ImGui::Text("Simulation control: ");
    ImGui::SameLine();
    if (_world->GetPhase() == World::Phase::Stopped) {
        if (ImGui::Button("Start")) {
            _world->SetPhase(World::Phase::Running);
        }
    } else if (_world->GetPhase() == World::Phase::Running) {
        if (ImGui::Button("Stop")) {
            _world->SetPhase(World::Phase::Stopped);
        }
        ImGui::SameLine();
        if (ImGui::Button("Pause")) {
            _world->SetPhase(World::Phase::Paused);
        }
    } else if (_world->GetPhase() == World::Phase::Paused) {
        if (ImGui::Button("Resume")) {
            _world->SetPhase(World::Phase::Running);
        }
    }

    return MenuWidgetAction::KeepOpen;
}