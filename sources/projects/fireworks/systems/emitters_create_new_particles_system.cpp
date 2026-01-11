#include "generated/auto_emitters_create_new_particles_system.h"

#include "components/generated/auto_emitter.h"
#include "components/generated/auto_emitter_random.h"
#include "components/generated/auto_particle_init_request.h"
#include "components/generated/auto_particle_position.h"
#include "game_controller.h"

void EmittersCreateNewParticlesSystem::DoProcessComponents(EcsEntity /*id*/, const ParticlePosition& particlePosition, const EmitterRandom& emitterRandom, Emitter& emitter)
{
    const float frameTimeSec = static_cast<float>(_gameController->GetFrameTime().AsSeconds());
    emitter.restParticlesToEmit += emitter.particlesPerSecond * frameTimeSec;

    const float realPart = std::roundf(emitter.restParticlesToEmit);
    const int32_t particlesToEmit = static_cast<int32_t>(realPart);
    emitter.restParticlesToEmit -= realPart;

    const uint32_t randomSeed = emitterRandom.value;
    static constexpr uint32_t NewParticlePrime { 4078736633  };

    for (int32_t i = 0; i < particlesToEmit; ++i) {
        const EcsEntity particle = _ecsWorld->create();
        const uint32_t particleRandomSeed = randomSeed * NewParticlePrime * (i + 1);
        _ecsWorld->emplace<ParticleInitRequest>(particle, particleRandomSeed, emitter.generation, particlePosition.x, particlePosition.y);
    }
}