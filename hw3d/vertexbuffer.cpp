#include "vertexbuffer.h"

VertexBuffer::VertexBuffer(Graphics& gfx, const std::vector<Vertex> vertices) {
	STARTUP();
	D3D11_BUFFER_DESC vbd = {};
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.CPUAccessFlags = 0u;
	vbd.MiscFlags = 0u;
	vbd.ByteWidth = (UINT)(sizeof(Vertex) * vertices.size());
	vbd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices.data();
	GFX_THROW(GetDevice(gfx)->CreateBuffer(&vbd, &sd, &vertexBuffer));
}

void VertexBuffer::Bind(Graphics& gfx) noexcept {
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	GetContext(gfx)->IASetVertexBuffers(0u, 1u, vertexBuffer.GetAddressOf(), &stride, &offset);
}