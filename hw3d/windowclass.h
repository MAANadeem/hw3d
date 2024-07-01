#pragma once

#include "mainwin.h"

class WindowClass {
private:
	//nested class handles window startup
	class SingleWindow {
	public:
		static const wchar_t* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		SingleWindow() noexcept;
		~SingleWindow();
		SingleWindow(const SingleWindow&) = delete;
		SingleWindow& operator=(const WindowClass&) = delete;
		static constexpr const wchar_t* winName = L"Engine";
		static SingleWindow win;
		HINSTANCE hInst;
	};
public:
	WindowClass(int, int, const wchar_t*) noexcept;
	~WindowClass();
private:
	static LRESULT CALLBACK HandleMsgSetup(HWND, UINT, WPARAM, LPARAM) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND, UINT, WPARAM, LPARAM) noexcept;
	LRESULT HandleMsg(HWND, UINT, WPARAM, LPARAM) noexcept;
private:
	int width;
	int height;
	HWND hwnd;
};