#define GLM_FORCE_SWIZZLE
#include "Camera.h"
#include <glm/ext.hpp>

Camera::Camera() :
	transform(m_transform),
	view(m_view),
	projection(m_projection),
	m_fov(0),
	m_aspectRatio(0),
	m_near(0),
	m_far(0),
	m_top(100),
	m_bottom(-100),
	m_left(-100),
	m_right(100),
	m_world(1),
	m_view(1),
	m_projection(1),
	m_target(0),
	m_projectionView(1),
	projectionView(m_projectionView)
{
}

Camera::~Camera()
{
}

void Camera::update(float deltatime)
{
	m_view = glm::inverse(m_transform.World);	
	m_projectionView = m_projection * m_view;
}

void Camera::setPerspective(float fieldOfView, float aspectRatio, float near, float far)
{
	m_fov = fieldOfView;
	m_aspectRatio = aspectRatio;
	m_near = near;
	m_far = far;

	const float x = 1.f / (m_aspectRatio * tan(m_fov / 2.f));
	const float y = 1.f / tan(m_fov / 2.f);
	const float z = -1.f * ((m_far + m_near) / (m_far - m_near));
	const float w = -1.f * ((2.f * m_far * m_near) / (m_far - m_near));

	const glm::mat4 projection = glm::mat4(
		glm::vec4(x, 0, 0, 0),//xcol
		glm::vec4(0, y, 0, 0),//ycol
		glm::vec4(0, 0, z, -1),//zcol
		glm::vec4(0, 0, w, 0));//wcol or translation

	auto expected = glm::perspective(fieldOfView, aspectRatio, near, far);

	assert(expected == projection);

	m_projection = projection;
}

void Camera::setLookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 up)
{
	m_target = target;
	const glm::vec3 z = normalize(eye - target);
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
	glm::mat4 expected = lookAt(eye, target, up);
	assert(actual == expected);	
	m_view = V * T;
	m_world = glm::inverse(m_view);
	m_transform.setWorld(m_world);	
}

void Camera::setOrthographic(float left, float right, float bottom, float top, float near, float far)
{
	const float sx = 2 / (right - left);
	const float sy = 2 / (top - bottom);
	const float sz = 2 / (far - near);

	const float tx = (left + right) / -2.f;
	const float ty = (top + bottom) / -2.f;
	const float tz = (far + near) / -2.f;

	const glm::mat4 S = {
		sx, 0, 0, 0,
		0, sy, 0, 0,
		0, 0, sz, 0,
		0, 0, 0, 1
	};

	const glm::mat4 T = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		tx, ty, tz, 1
	};

	glm::mat4 actual = S * T;
	auto expected = glm::ortho(left, right, bottom, top, near, far);
	//assert(actual == expected);
	m_projection = actual;
}

void Camera::setProjection(bool perspective)
{
	perspective
		? setPerspective(m_fov, m_aspectRatio, m_near, m_far)
		: setOrthographic(m_left, m_right, m_bottom, m_top, m_near, m_far);
}
