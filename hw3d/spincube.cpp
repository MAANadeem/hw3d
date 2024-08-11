#pragma once
#include "spincube.h"

SpinningCubes::SpinningCubes(Graphics& gfx) {
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> ddist(1.0f, 3.0f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 50; i++)
	{
		objects.push_back(std::make_unique<Cube>(
			gfx, rdist(rng), rdist(rng), rdist(rng),
			ddist(rng), ddist(rng), ddist(rng)
		));
	}
	gfx.SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 80.0f));
	gfx.SetCamera(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f));
}

void SpinningCubes::Update(float dt, WindowClass& wnd) {
	wnd.Gfx().ClearBuffer(0.0f, 0.0f, 0.0f);
	cam.Update(wnd);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	for (auto& o : objects) {
		o->Update(dt);
	}
}

void SpinningCubes::Draw(Graphics& gfx) {
	for (auto& o : objects) {
		o->Draw(gfx);
	}
}