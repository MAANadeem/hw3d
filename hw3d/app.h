#pragma once
#include "windowclass.h"
#include "timer.h"
#include "cube.h"
#include "cylinder.h"
#include "scene.h"
#include "spincube.h"
#include "spincyl.h"
#include "waterwaves.h"

class App {
public:
	App();
	//main loop
	int Run();
private:
	void Frame();
private:
	WindowClass wnd;
	Timer timer;
	std::vector<std::unique_ptr<Scene>> scenes;
	int sceneNum = 0;
	bool tabPressed = true;
};