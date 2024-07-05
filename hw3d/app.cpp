#include "app.h"

App::App() : wnd(800, 600, L"Hardware 3D") {}
int App::Run() {
	while (true) {
		if (const auto ecode = WindowClass::ProcessMessages()) {
			return *ecode;
		}
		Frame();
	}
}

void App::Frame() {
	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().ClearBuffer(1.0f, 1.0, 1.0f);
	wnd.Gfx().DrawTestTriangle(-timer.Peek(),
		0.0f,
		0.0f
	);
	wnd.Gfx().DrawTestTriangle(timer.Peek(),
		wnd.mouse.GetPosX() / 400.0f - 1.0f,
		-wnd.mouse.GetPosY() / 300.0f + 1.0f
	);
	wnd.Gfx().EndFrame();
}