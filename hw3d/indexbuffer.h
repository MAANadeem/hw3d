#pragma once
#include "bindable.h"

class IndexBuffer : public Bindable {
public:
	IndexBuffer(Graphics&, const std::vector<unsigned short>);
	void Bind(Graphics&) noexcept override;
	UINT GetCount() noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
	UINT count;
};