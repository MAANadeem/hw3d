#pragma once
#include "bindable.h"

class PixelShader : public Bindable {
public:
	PixelShader(Graphics&, const wchar_t*);
	void Bind(Graphics&) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
};