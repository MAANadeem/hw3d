#include "cube.h"

Cube::Cube(Graphics& gfx, float x, float y, float z, float width, float height, float depth, int col)
	:
	x(x),
	y(y),
	z(z),
	width(width),
	height(height),
	depth(depth)
{
	//change vertices to match dimensions
	const float sWidth = width / 2.0f;
	const float sHeight = height / 2.0f;
	const float sDepth = depth / 2.0f;

	const std::vector<Vertex> vertices =
	{
		{ -sWidth, -sHeight, -sDepth },
		{  sWidth, -sHeight, -sDepth },
		{ -sWidth,  sHeight, -sDepth },
		{  sWidth,  sHeight, -sDepth },
		{ -sWidth, -sHeight,  sDepth },
		{  sWidth, -sHeight,  sDepth },
		{ -sWidth,  sHeight,  sDepth },
		{  sWidth,  sHeight,  sDepth },
	};

	AddBind(std::make_unique<VertexBuffer>(gfx, vertices));

	auto pvs = std::make_unique<VertexShader>(gfx, L"vertexshader.cso");
	auto pvsbc = pvs->GetByteCode();
	AddBind(std::move(pvs));

	AddBind(std::make_unique<PixelShader>(gfx, L"pixelshader.cso"));

	const std::vector<unsigned short> indices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
	AddBind(std::make_unique<IndexBuffer>(gfx, indices));
	indexCount = (UINT)indices.size();

	struct alignas(16) Colors
	{
		int index;
		float padding[3];
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	const Colors cb2 =
	{ 
		col,
		{0.0f, 0.0f, 0.0f},
		{
			{ 1.0f,0.0f,1.0f,1.0f },
			{ 1.0f,0.0f,0.0f,1.0f },
			{ 0.0f,1.0f,0.0f,1.0f },
			{ 0.0f,0.0f,1.0f,1.0f },
			{ 1.0f,1.0f,0.0f,1.0f },
			{ 0.0f,1.0f,1.0f,1.0f },
		}
	};
	AddBind(std::make_unique<ConstantBuffer<Colors>>(gfx, 'p', cb2));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
	AddBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

	AddBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	AddBind(std::make_unique<TransBuffer>(gfx, *this));
}


void Cube::Update(float dt) noexcept {
	roll += droll * dt;
	pitch += dpitch * dt;
	yaw += dyaw * dt;
	x += dx * dt;
	y += dy * dt;
	z -= dz * dt;
}

void Cube::Rotate(float velocity) {
	namespace dx = DirectX;
	dx::XMFLOAT4 newCoords;
	dx::XMVECTOR temp = dx::XMVectorSet(x, y, z, 1.0f);
	temp = dx::XMVector4Transform(temp, dx::XMMatrixRotationY(velocity));
	dx::XMStoreFloat4(&newCoords, temp);
	x = newCoords.x; y = newCoords.y, z = newCoords.z;
}

DirectX::XMMATRIX Cube::GetTransformXM() const noexcept {
	return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		DirectX::XMMatrixTranslation(x, y, z);
}

unsigned int Cube::GetID() { return id; }
void Cube::SetID(unsigned int newID) { id = newID; }