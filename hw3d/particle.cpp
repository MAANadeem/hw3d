#include "particle.h"

Particle::Particle(Graphics& gfx, unsigned int num)
	: numCubes(num) {
	
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> ddist(0.0f, 0.0f);
	std::uniform_real_distribution<float> odist(0.0f, 0.0f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

	for (int i = 0; i < numCubes; ++i) {
		particles.push_back(std::make_unique<Cube>(
			gfx, rng,
			ddist, odist, rdist
		));
	}
	gfx.SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	gfx.SetCamera(DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f));
}

void Particle::Update(float dt, WindowClass& wnd) {
	wnd.Gfx().ClearBuffer(0.0f, 0.0f, 0.0f);
	cam.Update(wnd);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	for (auto& p : particles) {
		p->Rotate(velocity);
	}
}

void Particle::Draw(Graphics& gfx) {
	for (auto& p : particles) {
		p->Draw(gfx);
	}
}