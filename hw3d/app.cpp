#include "app.h"

App::App() : wnd(800, 600, L"Hardware 3D") {
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);
	for (auto i = 0; i < 50; i++)
	{
		cylinders.push_back(std::make_unique<Cylinder>(
			wnd.Gfx(), rng, adist,
			ddist, odist, rdist, 5
		));
	}
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}
int App::Run() {
	while (true) {
		if (const auto ecode = WindowClass::ProcessMessages()) {
			return *ecode;
		}
		Frame();
	}
}

void App::Frame() {
	auto dt = timer.Mark();
	wnd.Gfx().ClearBuffer(0.0f, 0.0f, 0.0f);
	for (auto& c : cylinders) {
		c->Update(dt);
		c->Draw(wnd.Gfx());
	}
	wnd.Gfx().EndFrame();
}