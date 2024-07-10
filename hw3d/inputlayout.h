#pragma once
#include "bindable.h"

class InputLayout : public Bindable {
public:
	InputLayout(Graphics&,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>&,
		ID3DBlob*);
	void Bind(Graphics&) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
};