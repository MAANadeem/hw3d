#pragma once
#include "scene.h"

class Waves : public Scene {
public:
	Waves(Graphics& gfx);
	void Draw(Graphics& gfx) override;
	void Update(float dt, WindowClass& wnd) override;
};