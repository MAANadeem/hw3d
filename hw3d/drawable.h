#pragma once
#include "graphics.h"
#include <DirectXMath.h>

class Bindable;

class Drawable {
public:
	void Draw(Graphics& gfx) const noexcept;
	void AddBind(std::unique_ptr<Bindable> bind) noexcept;
	virtual void Update(float dt) noexcept = 0;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	virtual ~Drawable() = default;
protected:
	UINT indexCount;
private:
	std::vector<std::unique_ptr<Bindable>> binds;
};