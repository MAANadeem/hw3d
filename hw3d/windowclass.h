#pragma once

#include "mainwin.h"
#include "basicerror.h"

class WindowClass {
public:
	class Exception : public BasicError{
	public:
		Exception(int, const char*, HRESULT) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept;
		static std::string TranslateErrorCode(HRESULT) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT hr;
	};
private:
	//singleton pattern handles window startup
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

//helper macro for error exception
#define WND_EXCEPT(hr) WindowClass::Exception( __LINE__, __FILE__, hr)