#pragma once
#include "windowclass.h"
#include "timer.h"
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
};