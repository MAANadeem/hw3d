#include "camera.h"

namespace dx = DirectX;
DirectX::XMMATRIX Camera::GetMatrix() const {
	const auto pos = dx::XMVector3Transform(
		dx::XMVectorSet(0.0f, 0.0f, -r, 0.0f),
		dx::XMMatrixRotationRollPitchYaw(phi, -theta, 0.0f)
	);
	return dx::XMMatrixLookAtLH(
		pos, dx::XMVectorZero(),
		dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)) *
		dx::XMMatrixRotationRollPitchYaw(pitch, -yaw, roll);
}

void Camera::Update(WindowClass& wnd) {
	wnd.kbd.TurnOnAuto();
	Keyboard::Event pressed = wnd.kbd.ReadKey();
	int movedX = wnd.mouse.GetPos().first;
	int movedY = wnd.mouse.GetPos().second;
	pitch = movedY / 100.0f;
	yaw = movedX / 100.0f;
	switch (pressed.GetCode()) {
	case VK_LEFT: theta -= 0.05; break;
	case VK_RIGHT: theta += 0.05; break;
	case VK_UP: phi += 0.05; break;
	case VK_DOWN: phi -= 0.05; break;
	case 0x41: yaw -= 0.05; break; //A key
	case 0x44: yaw += 0.05; break; //D key
	case 0x57: pitch += 0.05; break; //W key
	case 0x53: pitch -= 0.05; break; //S key
	}
}

void Camera::Reset() {
	r = 20.0f;
	theta = 0.0f;
	phi = 0.0f;

	pitch = 0.0f;
	roll = 0.0f;
	yaw = 0.0f;
}