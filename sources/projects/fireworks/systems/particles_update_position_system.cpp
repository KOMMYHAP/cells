#include "generated/auto_update_particle_position_system.h"

#include "components/generated/auto_particle_position.h"
#include "components/generated/auto_particle_velocity.h"
#include "game_controller.h"
#include "game_config.h"

void UpdateParticlePositionSystem::DoProcessComponents(EcsEntity id, const ParticleVelocity& particleVelocity, ParticlePosition& particlePosition)
{
    const float delta = particleVelocity.value * _gameController->GetFrameTime().AsSeconds();
    particlePosition.x += delta;
    particlePosition.y += delta;

    const float xMin = -_gameConfig->ui.worldWidgetSizeX;
    const float yMin = -_gameConfig->ui.worldWidgetSizeY;
    const float xMax = 2 * _gameConfig->ui.worldWidgetSizeX;
    const float yMax = 2 * _gameConfig->ui.worldWidgetSizeY;
    const bool insideByX = particlePosition.x >= xMin && particlePosition.x < xMax;
    const bool insideByY = particlePosition.y >= yMin && particlePosition.y < yMax;
    const bool inside = insideByX && insideByY;
    if (!inside) {
        _ecsWorld->destroy(id);
    }
}