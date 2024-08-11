#pragma once
#include "drawable.h"
#include "pipelineparts.h"

class Cube : public Drawable
{
public:
	Cube(Graphics& gfx, float, float, float, float, float, float, int);
	void Update(float dt) noexcept override;
	void Rotate(float velocity);
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
	
	unsigned int GetID();
	void SetID(unsigned int newID);
private:
	unsigned int id;
	// positional
	float x, y, z;
	float width, height, depth;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	// speed (delta/s)
	float droll = 0.0f;
	float dpitch = 0.0f;
	float dyaw = 0.0f;
	float dx = 0.0f, dy = 0.0f, dz = 0.0f;
};