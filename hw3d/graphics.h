#pragma once
#include "mainwin.h"
#include <d3d11.h>
#include <wrl.h>
#include <vector>
#include <comdef.h>
#include <d3dcompiler.h>
#include "basicerror.h"
#include "macros.h"
#include <random>
#include <DirectXMath.h>

class Graphics {
	friend class Bindable;
public:
	class Exception : public BasicError {
	public:
		Exception(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	private:
		HRESULT hr = CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);
	};
public:
	Graphics(HWND hwnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	
	void EndFrame();
	void ClearBuffer(float, float, float) noexcept;

	DirectX::XMMATRIX GetProjection();
	DirectX::XMMATRIX GetCamera();
	void SetProjection(DirectX::XMMATRIX proj);
	void SetCamera(DirectX::XMMATRIX cam);

	void DrawIndexed(UINT indexCount);

	void DrawTestTriangle(float angle, float x, float y);
private:
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> dsv;
};