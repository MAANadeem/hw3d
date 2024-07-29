#pragma once
#include "scene.h"

class SpinningCyls : public Scene {
public:
	SpinningCyls(Graphics& gfx);
	void Draw(Graphics& gfx) override;
	void Update(float dt, WindowClass& wnd) override;
};