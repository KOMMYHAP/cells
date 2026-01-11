#include "generated/auto_particles_init_system.h"

#include "components/generated/auto_emitter_random.h"
#include "components/generated/auto_particle_color.h"
#include "components/generated/auto_particle_generation.h"
#include "components/generated/auto_particle_init_request.h"
#include "components/generated/auto_particle_lifetime.h"
#include "components/generated/auto_particle_position.h"
#include "components/generated/auto_particle_velocity.h"
#include "game_config.h"
#include "sdl_utils.h"

void ParticlesInitSystem::DoProcessComponents(EcsEntity id, const ParticleInitRequest& particleInitRequest)
{
    const GameConfig::FireworksConfig* fireworksConfig = _gameConfig->FindFireworks(particleInitRequest.generation);
    if (!fireworksConfig) {
        _ecsWorld->destroy(id);
        return;
    }

    _ecsWorld->emplace<ParticleColor>(
        id,
        PackColor(fireworksConfig->colorFrom),
        PackColor(fireworksConfig->colorTo),
        PackColor(fireworksConfig->colorFrom));
    _ecsWorld->emplace<ParticleGeneration>(id, particleInitRequest.generation);
    _ecsWorld->emplace<ParticleLifetime>(id, fireworksConfig->framesToLive, fireworksConfig->framesToLive, 0.0f);
    _ecsWorld->emplace<ParticlePosition>(id, particleInitRequest.x, particleInitRequest.y);

    static constexpr uint32_t PrimeSpeed { 4078736633 };
    static constexpr uint32_t PrimeAngle { 4078736647 };
    const float randomSpeedCoef = PrimeSpeed * particleInitRequest.randomSeed / static_cast<float>(std::numeric_limits<uint32_t>::max());
    const float speed = fireworksConfig->minSpeed + randomSpeedCoef * (fireworksConfig->maxSpeed - fireworksConfig->minSpeed);

    const float randomAngleCoef = PrimeAngle * particleInitRequest.randomSeed / static_cast<float>(std::numeric_limits<uint32_t>::max());
    const float angle = fireworksConfig->minAngle + randomAngleCoef * (fireworksConfig->maxAngle - fireworksConfig->minAngle);

    ParticleVelocity& velocity = _ecsWorld->emplace<ParticleVelocity>(id);
    velocity.valueX = std::cos(angle * 3.14f / 180.0f) * speed;
    velocity.valueY = std::sin(angle * 3.14f / 180.0f) * speed;
    _ecsWorld->remove<ParticleInitRequest>(id);
}