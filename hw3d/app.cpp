#include "app.h"

App::App() : wnd(600, 400, L"Hardware 3D") {}
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
	wnd.Gfx().ClearBuffer(c, c, 1.0f);
	wnd.Gfx().EndFrame();
}