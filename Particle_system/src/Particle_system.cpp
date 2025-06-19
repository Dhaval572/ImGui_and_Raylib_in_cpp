#include "Particle_system.h"

// Helper function
static float RandomFloat(float min, float max)
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);
	return dis(gen);
}

ParticleSystem::ParticleSystem(Vector2 emitPos, int numParticles, bool isLoop, Color color)
	: isLoop(isLoop)
{
	particles.reserve(numParticles);

	for (int i = 0; i < numParticles; ++i)
	{
		Vector2 randomPos = {RandomFloat(emitPos.x - 100, emitPos.x + 100), RandomFloat(emitPos.y - 100, emitPos.y + 100)};

		particles.emplace_back(Particle(randomPos, {10, 10}, {10, 10}, 900.0f, 0.0f, 10.0f, 2.0f, color));
	}
}

void ParticleSystem::Update(float dt)
{
	for (auto &p : particles)
	{
		if (p.IsAlive())
			p.Update(dt);
	}

	if (!isLoop)
	{
		particles.erase(
			std::remove_if(particles.begin(), particles.end(),
						   [](const Particle &p)
						   { return !p.IsAlive(); }),
			particles.end());
	}
	else
	{
		for (auto &p : particles)
		{
			if (!p.IsAlive())
				p.Reset();
		}
	}
}

void ParticleSystem::Draw() const
{
	for (auto &p : particles)
	{
		if (p.IsAlive())
			p.Draw();
	}
}