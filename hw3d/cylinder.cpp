#include "cylinder.h"

Cylinder::Cylinder(Graphics& gfx,
	std::mt19937& rng,
	std::uniform_real_distribution<float>& adist,
	std::uniform_real_distribution<float>& ddist,
	std::uniform_real_distribution<float>& odist,
	std::uniform_real_distribution<float>& rdist,
	const int div)
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
	div(div)
{
	const auto bottomBase = DirectX::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
	const float rotAngle = 2.0f * PI / div;
	std::vector<Vertex> vertices;

	for (int i = 0; i < div; ++i) {
		DirectX::XMFLOAT3 v;
		auto dxv = DirectX::XMVector3Transform(bottomBase, DirectX::XMMatrixRotationZ(rotAngle * i));
		DirectX::XMStoreFloat3(&v, dxv);
		vertices.emplace_back(v.x, v.y, v.z);			//bottom side
		vertices.emplace_back(v.x, v.y, v.z + 2.0f);	//top side
	}

	vertices.emplace_back(0.0f, 0.0f, -1.0f);
	vertices.emplace_back(0.0f, 0.0f, 1.0f);
	const short iBottomCenter = (unsigned short)(vertices.size() - 2);
	const short iTopCenter = iBottomCenter + 1;

	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"vertexshader.cso");
	auto pvsbc = pvs->GetByteCode();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"pixelshader.cso"));

	std::vector<unsigned short> indices;
	for (int i = 0; i < div * 2; i += 2) {
		//bottom face tris
		indices.push_back(iBottomCenter);
		indices.push_back((i + 2) % (div*2));
		indices.push_back(i);
		//top face tris
		indices.push_back(iTopCenter);
		indices.push_back(i + 1);
		indices.push_back((i + 3) % (div * 2));
	}
	for (int i = 0; i < div * 2; i += 2) {
		//half quad
		indices.push_back(i);
		indices.push_back((i + 2) % (div * 2));
		indices.push_back((i + 1) % (div * 2));
		//other half
		indices.push_back((i + 1) % (div * 2));
		indices.push_back((i + 2) % (div * 2));
		indices.push_back((i + 3) % (div * 2));
	}

	AddBind(std::make_unique<IndexBuffer>(gfx, indices));
	indexCount = (UINT)indices.size();

	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	const ConstantBuffer2 cb2 =
	{
		{
			{ 1.0f,0.0f,1.0f },
			{ 1.0f,0.0f,0.0f },
			{ 0.0f,1.0f,0.0f },
			{ 0.0f,0.0f,1.0f },
			{ 1.0f,1.0f,0.0f },
			{ 0.0f,1.0f,1.0f },
		}
	};
	AddBind(std::make_unique<ConstantBuffer<ConstantBuffer2>>(gfx, 'p', cb2));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<TransBuffer>(gfx, *this));
}


void Cylinder::Update(float dt) noexcept {
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	theta += dtheta * dt;
	phi += dphi * dt;
	chi += dchi * dt;
}

DirectX::XMMATRIX Cylinder::GetTransformXM() const noexcept {
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(r, 0.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(theta, phi, chi) *
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 20.0f);
}

/*
/*
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
			vertices[ia[0]].SetTexture(0.0f, 0.0f);
			vertices[ia[1]].SetTexture(1.0f, 0.0f);
			vertices[ia[2]].SetTexture(0.0f, 1.0f);
			vertices[ia[3]].SetTexture(1.0f, 1.0f);

			indices.push_back(ia[0]);
			indices.push_back(ia[2]);
			indices.push_back(ia[1]);

			indices.push_back(ia[1]);
			indices.push_back(ia[2]);
			indices.push_back(ia[3]);
		}
	}

	auto tex = std::make_unique<Texture>(gfx, L"16-Bit-Mario.png");

	AddBind(std::move(tex));

	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));

	AddBind(std::make_unique<Sampler>(gfx));

	auto pvs = std::make_unique<VertexShader>(gfx, L"vertexshader.cso");
	auto pvsbc = pvs->GetByteCode();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"pixelshader.cso"));

	AddBind(std::make_unique<IndexBuffer>(gfx, indices));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		//{ "TexCoord",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<TransBuffer>(gfx, *this));
}
*/