#pragma once
#include "gl_core_4_4.h"
#include <mat4x4.hpp>
#include <vec3.hpp>
#include <vec4.hpp>


class Camera

{
public:
	Camera();
	~Camera();
	void update(float deltatime = 0);
	void setPerspective(float FOV, float AR, float m_near, float m_far);
	void setLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up);
	void setPosition(glm::vec3 position);
	void setOrtho(float left, float right, float top, float bottom, float farClip, float nearClip);
	glm::mat4 Getview();
	glm::mat4 getWorldTransform();
	glm::vec3 m_position;
	glm::mat4 m_view;
	glm::vec3 focus;
	glm::mat4 getProjection();
	glm::mat4 getProjectionView();

private:
	void updateProjectionViewTransform();
	glm::mat4 worldTransform;
	glm::mat4 viewTransform;
	glm::mat4 projectionTransform;
	glm::mat4 projectionViewTransform;
};