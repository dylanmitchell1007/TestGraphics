#include "Camera.h"
#include <glm/fwd.hpp>
#include <glm/ext.hpp>

Camera::Camera() {};
Camera::~Camera() {};
void Camera::update(float deltatime)
{
}

//FOV = Field of View || AR = Apsect Ratio || near = near ||
void Camera::setPerspective(float FOV, float AR, float m_near, float m_far)
{
	auto x = 1.f / (FOV * tan(FOV / 2.f));
	auto y = 1.f / tan(FOV / 2.f);
	auto z = -1.f * (m_far + m_near) / (m_far - m_near);
	auto w = -1.f * ((2.f * m_far * m_near) / (m_far - m_near));

	this->projectionTransform = glm::mat4(
		glm::vec4(x, 0, 0, 0),
		glm::vec4(0, y, 0, 0),
		glm::vec4(0, 0, z, -1.f),
		glm::vec4(0, 0, w, 0));

	auto copy = glm::perspective(FOV, AR, m_near, m_far);
	this->projectionTransform = copy;

}

void Camera::setLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	const glm::vec3 z = normalize(eye - center);
	const glm::vec3 x = normalize(cross(up, z));
	const glm::vec3 y = cross(z, x);

	const glm::mat4 V = glm::mat4(
		glm::vec4(x.x, y.x, z.x, 0),
		glm::vec4(x.y, y.y, z.y, 0),
		glm::vec4(x.z, y.z, z.z, 0),
		glm::vec4(0.0, 0.0, 0.0, 1)
	);

	const glm::mat4 T = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(-eye, 1)
	);

	glm::mat4 actual = V * T;
	glm::mat4 expected = glm::lookAt(eye, center, up);
	assert(actual == expected);
	auto view = V * T;
	auto world = glm::inverse(view);
	worldTransform = world;
	viewTransform = glm::inverse(world); 


}
void Camera::setPosition(glm::vec3 position)
{

}
glm::mat4 Camera::getWorldTransform()
{
	return this->worldTransform;
}
glm::mat4 Camera::getView()
{
	return this->viewTransform;
}
glm::mat4 Camera::getProjection()
{
	return this->projectionTransform;
}
glm::mat4 Camera::getProjectionView()
{
	return projectionTransform * viewTransform;
}

void Camera::updateProjectionViewTransform()
{

}