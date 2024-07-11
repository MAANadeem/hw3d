#pragma once
#include "windowclass.h"
#include "timer.h"
#include "cube.h"
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
	std::vector<std::unique_ptr<Cube>> cubes;
};