#pragma once
#include "Camera.h"
class FlyCamera : public Camera
{
private:
	float m_speed;
	glm::vec3 up;
public:
	FlyCamera();
	~FlyCamera();

	// Inherited via Camera
	virtual void update(float deltaTime) override;
	void setSpeed(float speed);
};
