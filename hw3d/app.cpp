#include "app.h"

App::App() : wnd(800, 600, L"Hardware 3D") {


	scenes.push_back(std::make_unique<Particle>(
		wnd.Gfx(), 6
	));

	scenes.push_back(std::make_unique<SpinningCubes>(
		wnd.Gfx()
	));


	/*
	scenes.push_back(std::make_unique<Waves>(
		wnd.Gfx()
	));
	scenes.push_back(std::make_unique<SpinningCyls>(
		wnd.Gfx()
	));
	*/
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
	scenes[sceneNum]->Update(dt, wnd);
	scenes[sceneNum]->Draw(wnd.Gfx());
	bool changeScene = wnd.kbd.KeyIsPressed(VK_TAB);
	if (changeScene && !tabPressed) {
		sceneNum = (sceneNum + 1) % scenes.size();
	}
	tabPressed = changeScene;
	wnd.Gfx().EndFrame();
}