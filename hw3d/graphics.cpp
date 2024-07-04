#include "graphics.h"
#include <d3dcompiler.h>

namespace wrl = Microsoft::WRL;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

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

	//ComPtr overloads the & operator, but with the added
	//side effect of calling Release() before giving the reference
	//if just the reference is needed, call GetAddress()
	D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
		nullptr, 0, D3D11_SDK_VERSION, &sd, &swap, &device,
		nullptr, &context
	);

	wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	swap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	device->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&target
	);
}
void Graphics::EndFrame() {
	swap->Present(2u, 0u);
}

void Graphics::ClearBuffer(float r, float g, float b) noexcept {
	const float color[] = { r, g, b, 1.0f };
	context->ClearRenderTargetView(target.Get(), color);
}

void Graphics::DrawTestTriangle() {
	namespace wrl = Microsoft::WRL;

	struct Vertex { 
		float x, y; 
		float r, g, b;
	};
	const Vertex vertices[] = {
		{0.0f, 0.5, 1.0f, 0.0f, 0.0f},
		{0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
		{-0.5f, -0.5f, 0.0f, 0.0f, 1.0f}
	};

	//create vertex buffer
	wrl::ComPtr<ID3D11Buffer> vertexBuffer;
	D3D11_BUFFER_DESC vbd = {};
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.CPUAccessFlags = 0u;
	vbd.MiscFlags = 0u;
	vbd.ByteWidth = sizeof(vertices);
	vbd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	device->CreateBuffer(&vbd, &sd, &vertexBuffer);

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

	//create and bind pixel shader
	wrl::ComPtr<ID3D11PixelShader> ps;
	wrl::ComPtr<ID3DBlob> b;
	D3DReadFileToBlob(L"pixelshader.cso", &b);
	device->CreatePixelShader(b->GetBufferPointer(),
		b->GetBufferSize(), nullptr, &ps);
	context->PSSetShader(ps.Get(), nullptr, 0u);
	
	//create and bind vertex shader
	wrl::ComPtr<ID3D11VertexShader> vs;
	D3DReadFileToBlob(L"vertexshader.cso", &b);
	device->CreateVertexShader(b->GetBufferPointer(),
		b->GetBufferSize(), nullptr, &vs);
	context->VSSetShader(vs.Get(), nullptr, 0u);

	//input layout
	wrl::ComPtr<ID3D11InputLayout> il;
	const D3D11_INPUT_ELEMENT_DESC ied[] = {
		{"Position", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	device->CreateInputLayout(
		ied, std::size(ied), 
		b->GetBufferPointer(), 
		b->GetBufferSize(), 
		&il
	);

	//bind input layout
	context->IASetInputLayout(il.Get());

	//bind render target
	context->OMSetRenderTargets(1, target.GetAddressOf(), nullptr);

	//set primitive topology
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context->RSSetViewports(1u, &vp);

	context->Draw(std::size(vertices), 0u);
}
