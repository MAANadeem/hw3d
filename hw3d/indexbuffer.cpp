#include "indexbuffer.h"

IndexBuffer::IndexBuffer(Graphics& gfx, const std::vector<unsigned short> indices) 
	: count((UINT)indices.size()) {
	STARTUP();
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = UINT(count * sizeof(unsigned short));
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA sd2 = {};
	sd2.pSysMem = indices.data();
	GFX_THROW(GetDevice(gfx)->CreateBuffer(&ibd, &sd2, &indexBuffer));
}

void IndexBuffer::Bind(Graphics& gfx) noexcept {
	GetContext(gfx)->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::GetCount() noexcept { return count; }