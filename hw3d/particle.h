#pragma once
#include "scene.h"

class Particle : public Scene {
public:
	Particle(Graphics& gfx, unsigned int num);
	void Update(float dt, WindowClass& wnd) override;
	void Draw(Graphics&) override;
	
private:
	unsigned int numCubes;
	float velocity = 0.01f;
	std::vector<std::unique_ptr<Cube>> particles;
};

