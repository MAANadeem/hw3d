#pragma once
#include "mainwin.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>

class Graphics {
public:
	Graphics(HWND hwnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	
	void EndFrame();
	void ClearBuffer(float, float, float) noexcept;

	void DrawTestTriangle(float angle, float x, float y);
private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv;
};