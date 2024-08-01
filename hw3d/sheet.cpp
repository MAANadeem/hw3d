#include "sheet.h"
#include "texture.h"
#include "sampler.h"

Sheet::Sheet(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	const int divX, const int divY)
	:
	r(rdist(rng)),
	droll(ddist(rng)),
	dpitch(ddist(rng)),
	dyaw(ddist(rng)),
	dphi(odist(rng)),
	dtheta(odist(rng)),
	dchi(odist(rng)),
	chi(adist(rng)),
	theta(adist(rng)),
	phi(adist(rng)),
	divX(divX), divY(divY)
{
	namespace dx = DirectX;

	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;

	const auto base = Vertex(-1.0f, -1.0f, 0.0f);
	float sizeX = 2.0 / (float)divX;
	float sizeY = 2.0 / (float)divY;
	int nVerticesX = divX + 1;
	int nVerticesY = divY + 1;

	for (int y = 0; y < nVerticesY; ++y) {
		for (int x = 0; x < nVerticesX; ++x) {
			Vertex v = Vertex(base.x + sizeX * x, base.y + sizeY * y, 0.0f);
			vertices.emplace_back(v);
		}
	}

	for (int y = 0; y < divY; ++y) {
		for (int x = 0; x < divX; ++x) {
			int ia[] = {
				y * nVerticesX + x, 
				y * nVerticesX + x + 1, 
				(y + 1) * nVerticesX + x, 
				(y + 1) * nVerticesX + x + 1
			};

			indices.push_back(ia[0]);
			indices.push_back(ia[2]);
			indices.push_back(ia[1]);

			indices.push_back(ia[1]);
			indices.push_back(ia[2]);
			indices.push_back(ia[3]);
		}
	}

	vertices[0].SetTexture(0.0f, 0.0f);
	vertices[1].SetTexture(1.0f * divX, 0.0f);
	vertices[2].SetTexture(0.0f, 1.0f * divY);
	vertices[3].SetTexture(1.0f*divX, 1.0f*divY);

	auto tex = std::make_unique<Texture>(gfx, L"16-Bit-Mario.png");

	AddBind(std::move(tex));

	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));

	AddBind(std::make_unique<Sampler>(gfx));

	auto pvs = std::make_unique<VertexShader>(gfx, L"textureVS.cso");
	auto pvsbc = pvs->GetByteCode();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"texturePS.cso"));

	AddBind(std::make_unique<IndexBuffer>(gfx, indices));
	indexCount = (UINT)indices.size();

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<TransBuffer>(gfx, *this));
}

void Sheet::Update(float dt) noexcept {
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Sheet::GetTransformXM() const noexcept {
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi);
}