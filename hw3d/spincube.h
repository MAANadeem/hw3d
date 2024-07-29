#pragma once
#include "scene.h"

class SpinningCubes : public Scene {
public:
	SpinningCubes(Graphics& gfx);
	void Draw(Graphics& gfx) override;
	void Update(float dt, WindowClass& wnd) override;
};