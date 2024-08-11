#include "particle.h"

Particle::Particle(Graphics& gfx, unsigned int num)
	: numCubes(num) {
	
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> ddist(1.0f, 3.0f);
	std::uniform_real_distribution<float> rdist(3.0f, 10.0f);

	for (int i = 0; i < numCubes; ++i) {
		particles.push_back(std::make_unique<Cube>(
			gfx, rdist(rng), rdist(rng), rdist(rng),
			ddist(rng), ddist(rng), ddist(rng), i
		));
		particles[i]->SetID(i + 1);
	}
	particles.push_back(std::make_unique<Cube>(
		gfx, 0.0f, 0.0f, 0.0f,
		10.0f, 0.5f, 10.5f, 0
	));
	gfx.SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	gfx.SetCamera(DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f));
}

void Particle::Update(float dt, WindowClass& wnd) {
	wnd.Gfx().ClearBuffer(0.0f, 0.0f, 0.0f);
	cam.Update(wnd);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	for (auto& p : particles) {
		if (p->GetID() != 0) p->Rotate(velocity);
	}
}

void Particle::Draw(Graphics& gfx) {
	for (auto& p : particles) {
		p->Draw(gfx);
	}
}