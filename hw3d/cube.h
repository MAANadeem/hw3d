#pragma once
#include "drawable.h"
#include "pipelineparts.h"

class Cube : public Drawable
{
public:
	Cube(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);
	void Update(float dt) noexcept override;
	void Rotate(float velocity);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	// positional
	float x, y, z;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dx, dy, dz;
};