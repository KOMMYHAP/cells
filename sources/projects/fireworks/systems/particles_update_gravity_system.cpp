#include "generated/auto_particles_update_gravity_system.h"

#include "components/generated/auto_particle_position.h"
#include "components/generated/auto_particle_velocity.h"
#include "game_controller.h"

void ParticlesUpdateGravitySystem::DoProcessComponents(EcsEntity /*id*/, const ParticlePosition& particlePosition, ParticleVelocity& particleVelocity)
{
    const GravityUnit gravityUnit = _gameController->GetGravityUnit();
    const double frameTimeSec = _gameController->GetFrameTime().AsSeconds();
    const double vectorX = static_cast<double>(gravityUnit.positionX) - particlePosition.x;
    const double vectorY = static_cast<double>(gravityUnit.positionY) - particlePosition.y;
    const double lengthSquare = vectorX * vectorX + vectorY * vectorY;
    const double acceleration = gravityUnit.constant * gravityUnit.mass / lengthSquare;
    const double accelerationPerFrame = acceleration * frameTimeSec;

    const double length = std::sqrt(lengthSquare);
    const double directionX = vectorX / length;
    const double directionY = vectorY / length;
    particleVelocity.valueX += static_cast<float>(accelerationPerFrame * directionX);
    particleVelocity.valueY += static_cast<float>(accelerationPerFrame * directionY);
}