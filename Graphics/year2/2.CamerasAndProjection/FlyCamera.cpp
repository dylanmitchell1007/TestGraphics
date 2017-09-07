#include "FlyCamera.h"


FlyCamera::FlyCamera(): m_speed(0)
{
}


FlyCamera::~FlyCamera()
{
}

void FlyCamera::update(float deltaTime) 
{	
	Camera::update(deltaTime);
}

void FlyCamera::setSpeed(float speed)
{
	m_speed = speed;
}
