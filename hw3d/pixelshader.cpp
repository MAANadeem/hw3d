#include "pixelshader.h"

PixelShader::PixelShader(Graphics& gfx, const wchar_t* path) {
	STARTUP();
	Microsoft::WRL::ComPtr<ID3DBlob> byteCode;
	GFX_THROW(D3DReadFileToBlob(path, &byteCode));
	GFX_THROW(GetDevice(gfx)->CreatePixelShader(
		byteCode->GetBufferPointer(),
		byteCode->GetBufferSize(),
		nullptr,
		&pixelShader
	));
}

void PixelShader::Bind(Graphics& gfx) noexcept {
	GetContext(gfx)->PSSetShader(pixelShader.Get(), nullptr, 0u);
}
