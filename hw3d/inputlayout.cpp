#include "inputlayout.h"

InputLayout::InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, ID3DBlob* vsByteCode) {	STARTUP();
	GFX_THROW(GetDevice(gfx)->CreateInputLayout(layout.data(), (UINT)layout.size(),
		vsByteCode->GetBufferPointer(), vsByteCode->GetBufferSize(), &inputLayout));
}
void InputLayout::Bind(Graphics& gfx) noexcept {
	GetContext(gfx)->IASetInputLayout(inputLayout.Get());
}