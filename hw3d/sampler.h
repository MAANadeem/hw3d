#pragma once
#include "Bindable.h"

class Sampler : public Bindable {
public:
	Sampler(Graphics&);
	void Bind(Graphics&) noexcept override;
protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
};