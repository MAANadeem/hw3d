#include "windowclass.h"
#include <stdlib.h>

WindowClass::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	: BasicError(line, file), hr(hr) {}
const char* WindowClass::Exception::GetType() const noexcept { return "Window Exception"; }
HRESULT WindowClass::Exception::GetErrorCode() const noexcept { return hr; }
std::string WindowClass::Exception::GetErrorString() const noexcept { return TranslateErrorCode(hr); }

const char* WindowClass::Exception::what() const noexcept {
	std::ostringstream oss;
	oss << GetType() << std::endl;
	oss << "ERROR CODE: " << GetErrorCode() << std::endl;
	oss << "DESCRIPTION: " << GetErrorString() << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

std::string WindowClass::Exception::TranslateErrorCode(HRESULT hr) noexcept {
	LPVOID lpMsgBuf = nullptr;
	DWORD dwFormatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS;
	DWORD msgLen = FormatMessageW(dwFormatFlags, nullptr, hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR)lpMsgBuf, 0, nullptr);
	if (msgLen == 0) return "Unidentified Error Code";
	std::wstring ws = (LPWSTR)lpMsgBuf;
	std::string errorMsg(ws.begin(), ws.end());
	LocalFree(lpMsgBuf);
	return errorMsg;
}

//singleton design pattern requirement
WindowClass::SingleWindow WindowClass::SingleWindow::win;

WindowClass::SingleWindow::SingleWindow() noexcept : hInst(GetModuleHandle(nullptr)) {
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = nullptr;
	RegisterClassEx(&wc);
}
WindowClass::SingleWindow::~SingleWindow() {
	UnregisterClass(GetName(), GetInstance());
}
const wchar_t* WindowClass::SingleWindow::GetName() noexcept {
	return winName;
}
HINSTANCE WindowClass::SingleWindow::GetInstance() noexcept {
	return win.hInst;
}

WindowClass::WindowClass(int width, int height, const wchar_t* name) noexcept : width(width), height(height) {
	//calculating window size via client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	
	//create window
	hwnd = CreateWindow(
		SingleWindow::GetName(), name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, SingleWindow::GetInstance(), this);
	//show window
	ShowWindow(hwnd, SW_SHOWDEFAULT);
}
WindowClass::~WindowClass() {
	DestroyWindow(hwnd);
}

LRESULT WindowClass::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg) {
		// we don't want the DefProc to handle this message because
		// we want our destructor to destroy the window, so return 0 instead of break
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT CALLBACK WindowClass::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowClass* const pWnd = static_cast<WindowClass*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowClass::HandleMsgThunk));
		// forward message to window class handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT CALLBACK WindowClass::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
	// retrieve ptr to window class
	WindowClass* const pWnd = reinterpret_cast<WindowClass*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window class handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}