#pragma once
#include <vector>
#include <random>
#include "Particle.h"

class ParticleSystem
{
private:
	std::vector<class Particle> particles;
	bool isLoop;

public:
	ParticleSystem(Vector2 emitPos, int numParticles,bool isLoop, Color color);
	void Update(float dt);
	void Draw() const;
};
