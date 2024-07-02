#include "app.h"
#include <string>

//NOTES:
/*
When making a window with Win32, there's two important ideas:
Windows and Messages
To make a window, you need to register the window class
and then create an instance
The first requires a Descriptor to set up the window class
The second is a function call to actually make a window
*/

int CALLBACK WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	try {
		App{}.Run();
	}
	catch (const BasicError& e) {
		MessageBoxA(nullptr, e.what(), e.GetType(), MB_OK);
	}
	catch (const std::exception& e) {
		MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK);
	}
	catch (...) {
		MessageBoxA(nullptr, "No details", "Unknown Exception", MB_OK);
	}
}