#pragma once
#include <rlimGui.h>
#include "ParticleSystem.h"
#include "ImGuiCustomTheme.h"
void DrawParticleSystemUI(ParticleSystem &ps);

class App
{
public:
	~App();
	void Run();

private:
	void Init();
	void Update(float dt);
	void Render();
	void Shutdown();

	ParticleSystem particleSystem;
};
