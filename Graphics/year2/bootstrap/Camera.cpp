#include "Camera.h"
#include "glfw3.h"
#include "gl_core_4_4.h"
#include <cassert>
#include <ext.hpp>

Camera::Camera() {};
Camera::~Camera() {};
void Camera::update(float deltatime)
{
}

//FOV = Field of View || AR = Apsect Ratio || near = near ||
void Camera::setPerspective(float FOV, float AR, float m_near, float m_far)
{
	auto x = 1.f / (FOV * tan(FOV / 2.f));
	auto y = 1.f / (FOV * tan(FOV / 2.f));
	auto z = -1.f * ((2.f * m_far * m_near) / (m_far - m_near));
	auto w = -1.f * ((2.f * m_far * m_near) / (m_far - m_near));

	this->projectionTransform = glm::mat4(
		glm::vec4(x, 0, 0, 0),
		glm::vec4(0, y, 0, 0),
		glm::vec4(0, 0, z, -1.f),
		glm::vec4(0, 0, w, 0));
	auto copy = glm::perspective(FOV, 16 / 9.f, 0.1f, 10.f);
	assert(this->projectionTransform == copy);
	
}
void Camera::setOrtho(float left, float right, float top, float bottom, float farClip, float nearClip)
{
	auto X = 2 / (right - left);
	auto Y = 2 / (top - bottom);
	auto Z = -2 / (farClip - nearClip);

	auto Xx = -((right + left) / (right - left));
	auto Yy = -((top + bottom) / (top - bottom));
	auto Zz = -((farClip + nearClip) / (farClip - nearClip));

	glm::mat4 Projection = glm::mat4(
		glm::vec4(X, 0, 0, 0),
		glm::vec4(0, Y, 0, 0),
		glm::vec4(0, 0, Z, 0),
		glm::vec4(Xx, Yy, Zz, 1));

	glm::mat4 glmOrtho = glm::ortho(left, right, bottom, top, nearClip, farClip);

	assert(Projection == glmOrtho);

	this->projectionTransform = Projection;
	this->updateProjectionViewTransform();
}
void Camera::setLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up)
{
	glm::vec3 z = glm::normalize(eye - center);
	glm::vec3 x = glm::normalize(glm::cross(up, z));
	glm::vec3 y = glm::cross(z, x);
	glm::mat4 v = glm::mat4(
		x.x, y.x, z.x, 0,
		x.y, y.y, z.y, 0,
		x.z, y.z, z.z, 0,
		0, 0, 0, 1);


	glm::mat4 translation = glm::mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-eye.x, -eye.y, -eye.z, 1);
	glm::mat4 view = v * translation;
	m_view = view;
	glm::mat4 lookat = glm::lookAt(eye, center, up);
	assert(view == lookat);
	worldTransform = glm::inverse(view);

	

}
void Camera::setPosition(glm::vec3 position)
{
	auto t = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(position.x, position.y, position.z, 1));
	this->viewTransform = t * this->viewTransform;
	this->worldTransform = glm::inverse(this->viewTransform);
}
glm::mat4 Camera::getWorldTransform()
{
	return this->worldTransform;
}
glm::mat4 Camera::Getview()
{
	return this->viewTransform;
}
glm::mat4 Camera::getProjection()
{
	return this->projectionTransform;
}
glm::mat4 Camera::getProjectionView()
{
	return this->projectionViewTransform;
}

void Camera::updateProjectionViewTransform()
{
	this->projectionViewTransform = this->projectionTransform * this->viewTransform;
}