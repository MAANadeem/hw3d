#include "waterwaves.h"

Waves::Waves(Graphics& gfx) {
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 50; i++) {
		objects.push_back(std::make_unique<Sheet>(
			gfx, rng, adist,
			ddist, odist, rdist, 2, 2
		));
	}
	gfx.SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
	gfx.SetCamera(DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f));
}

void Waves::Update(float dt, WindowClass& wnd) {
	wnd.Gfx().ClearBuffer(0.0f, 0.0f, 0.0f);
	cam.Update(wnd);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	for (auto& o : objects) {
		if (!wnd.kbd.KeyIsPressed(VK_SPACE)) o->Update(dt);
	}
}

void Waves::Draw(Graphics& gfx) {
	for (auto& o : objects) {
		o->Draw(gfx);
	}
}