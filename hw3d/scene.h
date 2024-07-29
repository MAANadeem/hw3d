#pragma once
#include "windowclass.h"
#include "cube.h"
#include "cylinder.h"

class Scene {
public:
	virtual void Update(float dt, WindowClass& wnd) = 0;
	virtual void Draw(Graphics& gfx) = 0;
	virtual ~Scene() = default;
protected:
	std::vector<std::unique_ptr<Drawable>> objects;
};