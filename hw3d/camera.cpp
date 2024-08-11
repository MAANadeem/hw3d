#include "camera.h"

namespace dx = DirectX;
DirectX::XMMATRIX Camera::GetMatrix() const {
	const auto pos = dx::XMVectorSet(x, y, z, 0.0f);
	return dx::XMMatrixLookToLH(
		pos, dx::XMVector3TransformNormal(dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
			dx::XMMatrixRotationRollPitchYaw(-pitch, yaw, roll)),
		dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
}

void Camera::Update(WindowClass& wnd) {
	int movedX = wnd.mouse.GetPos().first;
	int movedY = wnd.mouse.GetPos().second;
	pitch = movedY / 100.0f;
	yaw = movedX / 100.0f;

	wnd.kbd.TurnOnAuto();
	bool pressed = !wnd.kbd.IsKeyEmpty();
	if (pressed) Move(wnd.kbd.ReadKey().GetCode());

}

void Camera::Reset() {
	x = 0.0f, y = 0.0f;
	z = -20.0f;

	pitch = 0.0f;
	roll = 0.0f;
	yaw = 0.0f;
}

void Camera::Move(const char key) {
	dx::XMVECTOR pos = dx::XMVectorSet(x, y, z, 0.0f);

	dx::XMVECTOR dir = dx::XMVector3TransformNormal(dx::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
		dx::XMMatrixRotationRollPitchYaw(-pitch, yaw, roll));
	dx::XMVECTOR perp = dx::XMVector3Normalize(dx::XMVector3Cross(dx::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), dir));

	switch (key) {
	case 0x41: pos = dx::XMVectorSubtract(pos, perp); break;
	case 0x44: pos = dx::XMVectorAdd(pos, perp); break;
	case 0x57: pos = dx::XMVectorAdd(pos, dir); break;
	case 0x53: pos = dx::XMVectorSubtract(pos, dir); break;
	}

	x = dx::XMVectorGetX(pos);
	y = dx::XMVectorGetY(pos);
	z = dx::XMVectorGetZ(pos);
}