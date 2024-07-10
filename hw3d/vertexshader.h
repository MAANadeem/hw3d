#pragma once
#include "bindable.h"

class VertexShader : public Bindable {
public:
	VertexShader(Graphics&, const wchar_t*);
	void Bind(Graphics&) noexcept override;
	ID3DBlob* GetByteCode() const noexcept;
protected:
	Microsoft::WRL::ComPtr<ID3DBlob> byteCode;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
};