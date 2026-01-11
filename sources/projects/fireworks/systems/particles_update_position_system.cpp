#include "generated/auto_particles_update_position_system.h"

#include "components/generated/auto_borders_inside_camera_tag.h"
#include "components/generated/auto_borders_outside_world_tag.h"
#include "components/generated/auto_particle_position.h"
#include "components/generated/auto_particle_velocity.h"
#include "game_config.h"
#include "game_controller.h"

void ParticlesUpdatePositionSystem::DoProcessComponents(EcsEntity id, const ParticleVelocity& particleVelocity, ParticlePosition& particlePosition)
{
    const float frameTimeSec = static_cast<float>(_gameController->GetFrameTime().AsSeconds());
    particlePosition.x += particleVelocity.valueX * frameTimeSec;
    particlePosition.y += particleVelocity.valueY * frameTimeSec;

    {
        const float xMin = static_cast<float>(-_gameConfig->ui.worldWidgetSizeX);
        const float yMin = static_cast<float>(-_gameConfig->ui.worldWidgetSizeY);
        const float xMax = static_cast<float>(2 * _gameConfig->ui.worldWidgetSizeX);
        const float yMax = static_cast<float>(2 * _gameConfig->ui.worldWidgetSizeY);
        const bool insideByX = particlePosition.x >= xMin && particlePosition.x < xMax;
        const bool insideByY = particlePosition.y >= yMin && particlePosition.y < yMax;
        const bool inside = insideByX && insideByY;
        if (inside) {
            _ecsWorld->remove<BordersOutsideWorldTag>(id);
        } else {
            _ecsWorld->emplace_or_replace<BordersOutsideWorldTag>(id);
        }
    }

    {
        const float xMin = 0.0f;
        const float yMin = 0.0f;
        const float xMax = static_cast<float>(_gameConfig->ui.worldWidgetSizeX);
        const float yMax = static_cast<float>(_gameConfig->ui.worldWidgetSizeY);
        const bool insideByX = particlePosition.x >= xMin && particlePosition.x < xMax;
        const bool insideByY = particlePosition.y >= yMin && particlePosition.y < yMax;
        const bool inside = insideByX && insideByY;
        if (inside) {
            _ecsWorld->emplace_or_replace<BordersInsideCameraTag>(id);
        } else {
            _ecsWorld->remove<BordersInsideCameraTag>(id);
        }
    }
}