#include "graphics.h"
#include <cmath>
#include <DirectXMath.h>
#include "macros.h"
#include "pipelineparts.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "D3DCompiler.lib")

Graphics::Graphics(HWND hwnd) {
	STARTUP();

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
	GFX_THROW(D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
		nullptr, 0, D3D11_SDK_VERSION, &sd, &swap, &device,
		nullptr, &context
	));

	wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	swap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	device->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&target
	);

	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	wrl::ComPtr<ID3D11DepthStencilState> dssState;
	device->CreateDepthStencilState(&dsDesc, &dssState);

	context->OMSetDepthStencilState(dssState.Get(), 1u);

	wrl::ComPtr<ID3D11Texture2D> depthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 800u;
	descDepth.Height = 600u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	device->CreateTexture2D(&descDepth, nullptr, &depthStencil);

	D3D11_DEPTH_STENCIL_VIEW_DESC ddsv = {};
	ddsv.Format = DXGI_FORMAT_D32_FLOAT;
	ddsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	ddsv.Texture2D.MipSlice = 0u;
	device->CreateDepthStencilView(depthStencil.Get(), &ddsv, &dsv);

	context->OMSetRenderTargets(1u, target.GetAddressOf(), dsv.Get());
}
void Graphics::EndFrame() {
	swap->Present(2u, 0u);
}

void Graphics::ClearBuffer(float r, float g, float b) noexcept {
	const float color[] = { r, g, b, 1.0f };
	context->ClearRenderTargetView(target.Get(), color);
	context->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::DrawTestTriangle(float angle, float x, float z) {
	namespace wrl = Microsoft::WRL;
	STARTUP();

	struct Vertex { 
		struct {
			float x, y, z;
		} pos;
	};
	const Vertex vertices[] = {
		{-1.0f, -1.0f, -1.0f},
		{1.0f, -1.0f, -1.0f},
		{-1.0f, 1.0f, -1.0f},
		{1.0f, 1.0f, -1.0f},
		{-1.0f, -1.0f, 1.0f},
		{1.0f, -1.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f},
	};
	unsigned int indices[] = {
		0,2,1,  2,3,1,
		1,3,5,  3,7,5,
		2,6,3,  3,6,7,
		4,5,7,  4,7,6,
		0,4,2,  2,4,6,
		0,1,4,  1,5,4
	};

	struct CBuffer {
		dx::XMMATRIX transform;
	};

	const CBuffer cb = {
		{
			dx::XMMatrixTranspose(
				dx::XMMatrixRotationZ(angle)*
				dx::XMMatrixRotationX(angle)*
				dx::XMMatrixTranslation(x, 0.0f, z + 4.0f) *
				dx::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
			)
		}
	};

	struct CB2 {
		struct{
			float r, g, b, a;
		} face_colors[6];
	};
	const CB2 cb2 = {
		{
			{1.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 1.0f},
		}
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
	GFX_THROW(device->CreateBuffer(&vbd, &sd, &vertexBuffer));

	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	context->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);

	//create and bind pixel shader
	auto ps1 = std::make_unique<PixelShader>(*this, L"pixelshader.cso");
	ps1->Bind(*this);

	//wrl::ComPtr<ID3D11VertexShader> vs;
	auto vs = std::make_unique<VertexShader>(*this, L"vertexshader.cso");
	vs->Bind(*this);

	//input layout
	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	auto il = std::make_unique<InputLayout>(*this, ied, vs->GetByteCode());

	//create index buffer
	wrl::ComPtr<ID3D11Buffer> indexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	D3D11_SUBRESOURCE_DATA sd2 = {};
	sd2.pSysMem = indices;
	device->CreateBuffer(&ibd, &sd2, &indexBuffer);

	//create constant buffer
	wrl::ComPtr<ID3D11Buffer> constantBuffer;
	D3D11_BUFFER_DESC cbd = {};
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA sd3 = {};
	sd3.pSysMem = &cb;
	device->CreateBuffer(&cbd, &sd3, &constantBuffer);

	//create second constant buffer
	wrl::ComPtr<ID3D11Buffer> constantBuffer2;
	D3D11_BUFFER_DESC cbd2 = {};
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(cb2);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA sd4 = {};
	sd4.pSysMem = &cb2;
	device->CreateBuffer(&cbd2, &sd4, &constantBuffer2);

	context->VSSetConstantBuffers(0u, 1u, constantBuffer.GetAddressOf());
	context->PSSetConstantBuffers(0u, 1u, constantBuffer2.GetAddressOf());

	//bind input layout
	il->Bind(*this);

	//bind index buffer
	context->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

	//set primitive topology
	auto tp = std::make_unique<Topology>(*this, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	tp->Bind(*this);

	//configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	context->RSSetViewports(1u, &vp);

	context->DrawIndexed(std::size(indices), 0u, 0u);
}

Graphics::Exception::Exception(int line, const char* file, HRESULT hr) noexcept 
: BasicError(line, file), hr(hr){}

const char* Graphics::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< "[Info] " << GetErrorInfo() << std::endl;
	oss << GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Graphics::Exception::GetType() const noexcept { return "Graphics Exception"; }
HRESULT Graphics::Exception::GetErrorCode() const noexcept{ return hr; }

std::string Graphics::Exception::GetErrorDescription() const noexcept {
	_com_error err(GetErrorCode());
	const wchar_t* wideErrorMsg = err.ErrorMessage();

	// Convert wchar_t* to std::string
	std::wstring wideStr(wideErrorMsg);
	std::string errorMsg(wideStr.begin(), wideStr.end());

	return errorMsg;
}

std::string Graphics::Exception::GetErrorInfo() const noexcept {
	_com_error err(GetErrorCode());
	IErrorInfo* info = err.ErrorInfo();
	BSTR des;
	if (SUCCEEDED(info->GetDescription(&des))) {
		std::wstring wideStr(des);
		std::string errorInfo(wideStr.begin(), wideStr.end());
		SysFreeString(des);
		return errorInfo;
	}
	else {
		return "No Info";
	}
}