#pragma once
#include "graphics.h"

class Bindable {
public:
	virtual void Bind(Graphics&) noexcept = 0;
	virtual ~Bindable() = default;
protected:
	static ID3D11Device* GetDevice(Graphics&) noexcept;
	static ID3D11DeviceContext* GetContext(Graphics&) noexcept;
};