#include "app.h"
#include <iomanip>

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
	
}