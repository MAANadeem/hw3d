#pragma once
#include "spincyl.h"

SpinningCyls::SpinningCyls(Graphics& gfx) {
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 50; i++)
	{
		objects.push_back(std::make_unique<Cylinder>(
			gfx, rng, adist,
			ddist, odist, rdist, 6
		));
	}
	gfx.SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

void SpinningCyls::Update(float dt, WindowClass& wnd) {
	wnd.Gfx().ClearBuffer(1.0f, 1.0f, 1.0f);
	for (auto& o : objects) {
		o->Update(dt);
	}
}

void SpinningCyls::Draw(Graphics& gfx) {
	for (auto& o : objects) {
		o->Draw(gfx);
	}
}