#include "bindable.h"

ID3D11Device* Bindable::GetDevice(Graphics& gfx) noexcept {
	return gfx.device.Get();
}
ID3D11DeviceContext* Bindable::GetContext(Graphics& gfx) noexcept {
	return gfx.context.Get();
}