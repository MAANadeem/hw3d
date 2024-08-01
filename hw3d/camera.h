#pragma once
#include "graphics.h"
#include "windowclass.h"
class Camera {
public:
	DirectX::XMMATRIX GetMatrix() const;
	void Update(WindowClass& wnd);
	void Reset();
private:
	float r = 20.0f;
	float theta = 0.0f;
	float phi = 0.0f;

	float pitch = 0.0f;
	float roll = 0.0f;
	float yaw = 0.0f;
};

