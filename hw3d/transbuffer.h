#pragma once
#include "constantbuffer.h"
#include "drawable.h"
#include <DirectXMath.h>

class TransBuffer : public Bindable
{
public:
	TransBuffer(Graphics& gfx, const Drawable& parent);
	void Bind(Graphics& gfx) noexcept override;
private:
	static std::unique_ptr<ConstantBuffer<DirectX::XMMATRIX>> vcbuf;
	const Drawable& parent;
};