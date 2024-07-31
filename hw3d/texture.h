#pragma once
#include "bindable.h"
#include "wictextureloader.h"

class Texture : public Bindable{
public:
	Texture(Graphics& gfx, const wchar_t* path);
	void Bind(Graphics& gfx) noexcept override;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureView;
};