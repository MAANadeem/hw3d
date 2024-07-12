#pragma once
#include <DirectXMath.h>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

struct Vertex {
	float x, y, z;
	Vertex(float x, float y, float z) : x(x), y(y), z(z) {}
};