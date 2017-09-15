#pragma once
#include "gl_core_4_4.h"
#include <vec3.hpp>

class FlyCamera
{
public:
	void update(float deltatime);
	void setSpeed(float speed);

private:
	FlyCamera();
	void speed();
	glm::vec3 up();
};