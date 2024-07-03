#pragma once
#include "mainwin.h"
#include <d3d11.h>

class Graphics {
public:
	Graphics(HWND hwnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();
	
	void EndFrame();
	void ClearBuffer(float, float, float) noexcept;
private:
	ID3D11Device* device;
	IDXGISwapChain* swap;
	ID3D11DeviceContext* context;
	ID3D11RenderTargetView* target;
};