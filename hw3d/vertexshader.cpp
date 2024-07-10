#include "vertexshader.h"

VertexShader::VertexShader(Graphics& gfx, const wchar_t* path) {
	STARTUP();
	GFX_THROW(D3DReadFileToBlob(path, &byteCode));
	GFX_THROW(GetDevice(gfx)->CreateVertexShader(
		byteCode->GetBufferPointer(),
		byteCode->GetBufferSize(), 
		nullptr, 
		&vertexShader
	));
}

void VertexShader::Bind(Graphics& gfx) noexcept {
	GetContext(gfx)->VSSetShader(vertexShader.Get(), nullptr, 0u);
}

ID3DBlob* VertexShader::GetByteCode() const noexcept {
	return byteCode.Get();
}
