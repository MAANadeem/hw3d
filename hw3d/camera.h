#pragma once
#include "graphics.h"
#include "windowclass.h"
class Camera {
public:
	DirectX::XMMATRIX GetMatrix() const;
	void Update(WindowClass& wnd);
	void Move(const char key);
	void Reset();
private:
	float x, y;
	float z = -10.0f;

	float pitch = 0.0f;
	float roll = 0.0f;
	float yaw = 0.0f;
};

