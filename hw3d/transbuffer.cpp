#include "transbuffer.h"

std::unique_ptr<ConstantBuffer<DirectX::XMMATRIX>> TransBuffer::vcbuf;

TransBuffer::TransBuffer(Graphics& gfx, const Drawable& parent) : parent(parent) {
	if (!vcbuf) {
		vcbuf = std::make_unique<ConstantBuffer<DirectX::XMMATRIX>>(gfx, 'v');
	}
}
void TransBuffer::Bind(Graphics& gfx) noexcept
{
	vcbuf->Update(gfx,
		DirectX::XMMatrixTranspose(
			parent.GetTransformXM() * gfx.GetProjection()
		)
	);
	vcbuf->Bind(gfx);
}