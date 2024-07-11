#pragma once
#include "bindable.h"

class VertexBuffer : public Bindable {
public:
	VertexBuffer(Graphics&, const std::vector<Vertex>);
	void Bind(Graphics&) noexcept;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
};