#include "graphics.h"

#pragma comment (lib, "d3d11.lib")

Graphics::Graphics(HWND hwnd) {
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
		nullptr, 0, D3D11_SDK_VERSION, &sd, &swap, &device,
		nullptr, &context
	);

	ID3D11Resource* pBackBuffer = nullptr;
	swap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
	device->CreateRenderTargetView(
		pBackBuffer,
		nullptr,
		&target
	);
	pBackBuffer->Release();

}

Graphics::~Graphics() {
	if (device != nullptr) {
		device->Release();
	}
	if (swap != nullptr) {
		swap->Release();
	}
	if (context != nullptr) {
		context->Release();
	}
	if (target != nullptr) {
		target->Release();
	}
}

void Graphics::EndFrame() {
	swap->Present(2u, 0u);
}

void Graphics::ClearBuffer(float r, float g, float b) noexcept {
	const float color[] = { r, g, b, 1.0f };
	context->ClearRenderTargetView(target, color);
}
