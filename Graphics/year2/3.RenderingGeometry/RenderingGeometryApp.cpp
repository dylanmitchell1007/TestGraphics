#define GLM_FORCE_SWIZZLE

#include "RenderingGeometryApp.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>
#include <glm/fwd.hpp>
#include <glm/common.hpp>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>

#include <Transform.h>
#include <Camera.h>
#include "Shader.h"
#include <iostream>


RenderingGeometryApp::RenderingGeometryApp() : m_runningTime(0), m_camera(nullptr), m_mouseX(0),
m_mouseY(0),
m_mouseButtonDown(false),
m_mouseButtonUp(false), m_isDragging(false), m_deltaX(0), m_deltaY(0), m_startX(0),
m_startY(0), m_endX(0),
m_endY(0),
m_angleAxis(true),
perspective(true)
{
}


RenderingGeometryApp::~RenderingGeometryApp()
{
}


void RenderingGeometryApp::startup()
{
	//setup camera info
	m_fov = glm::quarter_pi<float>();
	m_aspectRatio = 16 / 9.f;
	m_near = .1f;
	m_far = 1000.f;
	m_top = 50;
	m_bottom = -50;
	m_left = -50;
	m_right = 50;

	const glm::vec3 eye = glm::vec3(10, 10, 5);//camera in world is at positive 10
	const glm::vec3 target = glm::vec3(0);
	const glm::vec3 up = glm::vec3(0, 1, 0);

	m_camera = new Camera();
	m_camera->setPerspective(m_fov, m_aspectRatio, m_near, m_far);
	m_camera->setLookAt(eye, target, up);

	setKeyCallback(key_callback);
	setScrollCallback(scroll_callback);

	Vertex a = { glm::vec4(-0.5,  0, 0, 1)		, glm::vec4(1, 0, 0, .5) };
	Vertex b = { glm::vec4(0.5,  0, 0, 1)			, glm::vec4(0, 1, 0, .5) };
	Vertex c = { glm::vec4(0.5, -0.5, 0, 1)			, glm::vec4(0, 0, 1, .5) };
	Vertex d = { glm::vec4(-0.5, -0.5, 0, 1)		, glm::vec4(1, 0, 0, .5) };
	Vertex e = { glm::vec4(-0.5,  0.5, 0, 1)		, glm::vec4(0, 1, 0, .5) };

	std::vector<Vertex> myObject = { a, b, c, d, e };
	std::vector<unsigned int> objectIndices = { 0, 1, 2, 0, 2, 3, 0, 4, 1 };
	//create vertex array
	this->object = new Mesh();
	this->object->initialize(myObject, objectIndices);
	//create buffers
	this->object->create_buffers();
	//buffer data

	//setup vertex attributes

	//build shader
	this->myshader = new Shader();
	this->myshader->defaultLoad();
}

void RenderingGeometryApp::update(float delta_time)
{
	m_runningTime += delta_time;
	m_deltaTime = delta_time;
	m_mouseButtonDown = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS;
	m_mouseButtonUp = glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE;

	glfwGetCursorPos(m_window, &m_mouseX, &m_mouseY);
	//if clicked
	if (m_mouseButtonUp)
	{
		m_isDragging = false;
		m_deltaX = m_deltaY = 0;
		m_startX = m_startY = 0;
		m_endX = m_endY = 0;
	}

	if (m_mouseButtonDown)
	{
		if (m_isDragging == false)
		{
			m_isDragging = true;
			glfwGetCursorPos(m_window, &m_startX, &m_startY);
		}

		m_deltaX = (m_mouseX - m_startX);
		m_deltaY = (m_mouseY - m_startY);
	}

	static int width, height;

	glfwGetWindowSize(m_window, &width, &height);

	if (m_deltaY != 0)
	{
		float angle = (float)(m_deltaY / height) * delta_time;

		if (m_angleAxis)
			m_camera->transform.rotateAround(-angle, m_camera->transform.right.xyz());
		else
			m_camera->transform.rotate(angle, m_camera->transform.right.xyz());
	}

	if (m_deltaX != 0)
	{
		float angle = (float)(m_deltaX / width) * delta_time;

		if (m_angleAxis)
			m_camera->transform.rotateAround(-angle, glm::vec3(0, 1, 0));
		else
			m_camera->transform.rotate(angle, glm::vec3(0, 1, 0));
	}


	if (glfwGetKey(m_window, 'W'))
		m_camera->transform.translate(glm::vec3(0, 0, -10));				
	if (glfwGetKey(m_window, 'S'))
		m_camera->transform.translate(glm::vec3(0, 0, 10));
	if (glfwGetKey(m_window, 'A'))
		m_camera->transform.translate(glm::vec3(-10, 0, 0));
	if (glfwGetKey(m_window, 'D'))
		m_camera->transform.translate(glm::vec3(10, 0, 0));

	if (glfwGetKey(m_window, 'F'))
	{
		m_camera->setLookAt(m_camera->transform.World[3].xyz(), glm::vec3(0), glm::vec3(0, 1, 0));
	}

	(perspective) ?
		m_camera->setPerspective(m_fov, m_aspectRatio, m_near, m_far) :
		m_camera->setOrthographic(m_left, m_right, m_bottom, m_top, m_near, m_far);

	m_camera->update(delta_time);
	auto camPos = m_camera->view[3];
	
	system("cls");
	std::cout << camPos.x << "/" << camPos.y << "/" << camPos.z;
}

glm::mat4 aObject = glm::mat4(1);
void RenderingGeometryApp::draw()
{
	//draw object
	this->myshader->bind();

	auto translation = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(10, 10, 10, 1)
	);
	//aObject = aObject * translation;

	unsigned int projectionViewUniform = this->myshader->getUniform("ProjectionViewWorld");
	glUniformMatrix4fv(projectionViewUniform, 1, GL_FALSE, glm::value_ptr(m_camera->projectionView));
	this->object->draw(GL_TRIANGLES);
	this->myshader->unbind();

#pragma region UI

	ImGui_ImplGlfwGL3_NewFrame();
	ImGui::Begin("Menu", (bool*)0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	m_gameover = ImGui::Button("Quit?", ImVec2(100, 50));
	ImGui::End();


	ImGui::Begin("Rendering Options 2", (bool*)0, ImGuiWindowFlags_NoResize);
	ImGui::Text("Running Time: %f", m_runningTime);
	ImGui::Text("delta time : %f", m_deltaTime);
	ImGui::End();

#pragma endregion UI
}
std::vector<glm::vec3> halfcircle(float radius, unsigned int points)
{
	std::vector<glm::vec3> tmpArray;

	for (int i = 0; i < points; i++)
	{
		auto slice = 3.14 / (points - 1);
		auto angle = slice * i;
		glm::vec3 vertex = glm::vec3(cosf(angle), sinf(angle), 0);
		tmpArray.push_back(vertex);
	}
	return tmpArray;
}
void RenderingGeometryApp::shutdown()
{
}

void RenderingGeometryApp::setKeyCallback(KeyFunc func)
{
	glfwSetWindowUserPointer(m_window, this);
	glfwSetKeyCallback(m_window, func);
}

void RenderingGeometryApp::setScrollCallback(ScrollFunc func)
{
	glfwSetWindowUserPointer(m_window, this);
	glfwSetScrollCallback(m_window, func);
}

void RenderingGeometryApp::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	RenderingGeometryApp* instance = (RenderingGeometryApp*)glfwGetWindowUserPointer(window);
	instance->m_camera->transform.translate(glm::vec3(0, -yoffset, 0));
}

void RenderingGeometryApp::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	RenderingGeometryApp* instance = (RenderingGeometryApp*)glfwGetWindowUserPointer(window);
	Camera* camera = instance->m_camera;
	if (key == GLFW_KEY_SPACE)
		camera->setLookAt(glm::vec3(10, 10, -10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

}