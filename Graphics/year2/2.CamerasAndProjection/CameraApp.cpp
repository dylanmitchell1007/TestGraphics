#define GLM_FORCE_SWIZZLE
#include <Gizmos.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <Camera.h>
#include <Transform.h>
#include "CameraApp.h"


CameraApp::CameraApp() : m_runningTime(0), m_camera(nullptr), m_earth(nullptr), m_sun(nullptr), m_mouseX(0),
m_mouseY(0),
m_mouseButtonDown(false),
m_mouseButtonUp(false), m_isDragging(false), m_deltaX(0), m_deltaY(0), m_startX(0),
m_startY(0), m_endX(0),
m_endY(0),
m_angleAxis(true),
perspective(true)
{

}

CameraApp::~CameraApp()
{
}


void CameraApp::startup()
{
	m_sun = new Transform();
	m_earth = new Transform();
	m_camera = new Camera();


	m_fov = glm::quarter_pi<float>();
	m_aspectRatio = 16 / 9.f;
	m_near = .1f;
	m_far = 100;
	m_top = 50;
	m_bottom = -50;
	m_left = -50;
	m_right = 50;

	const glm::vec3 eye = glm::vec3(10, 10, 20);//camera in world is at positive 10
	const glm::vec3 target = glm::vec3(0);
	const glm::vec3 up = glm::vec3(0, 1, 0);

	m_camera->setPerspective(m_fov, m_aspectRatio, m_near, m_far);
	m_camera->setLookAt(eye, target, up);

	Gizmos::create();	

	setScrollCallback(scroll_callback);
	setKeyCallback(key_callback);
}

void CameraApp::shutdown()
{
	delete m_camera;
	ImGui_ImplGlfwGL3_Shutdown();
	Gizmos::destroy();
}

void CameraApp::update(float delta_time)
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

	static int width;
	static int height;
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
		
		if(m_angleAxis)
			m_camera->transform.rotateAround(-angle, glm::vec3(0, 1, 0));
		else
			m_camera->transform.rotate(angle, glm::vec3(0, 1, 0));
	}
		

	if (glfwGetKey(m_window, 'W'))
		m_camera->transform.translate(glm::vec3(0, 0, -1));
	if (glfwGetKey(m_window, 'S'))
		m_camera->transform.translate(glm::vec3(0, 0, 1));
	if (glfwGetKey(m_window, 'A'))
		m_camera->transform.translate(glm::vec3(-1, 0, 0));
	if (glfwGetKey(m_window, 'D'))
		m_camera->transform.translate(glm::vec3(1, 0, 0));	

	if (glfwGetKey(m_window, 'F'))
	{
		m_camera->setLookAt(m_camera->transform.World[3].xyz(), glm::vec3(0), glm::vec3(0, 1, 0));
	}

	m_sun->rotate(glm::pi<float>() / 180, glm::vec3(0, 1, 0));
	m_earth->setWorld(m_sun->World * glm::translate(glm::vec3(8, 0, 0)));
	(perspective) ?
		m_camera->setPerspective(m_fov, m_aspectRatio, m_near, m_far) :
		m_camera->setOrthographic(m_left, m_right, m_bottom, m_top, m_near, m_far);
	m_camera->update(delta_time);
}

void CameraApp::draw()
{
	Gizmos::clear();

	for (int i = 0; i <= 40; ++i)
	{
		Gizmos::addLine(glm::vec3(-20 + i, 0, 20), glm::vec3(-20 + i, 0, -20), WHITE);
		Gizmos::addLine(glm::vec3(20, 0, -20 + i), glm::vec3(-20, 0, -20 + i), BLACK);
	}

	Gizmos::addSphere(m_sun->World[3].xyz(), 3, 15, 15, glm::vec4(.5, .5, 0, .5), &m_sun->World);
	Gizmos::addSphere(m_earth->World[3].xyz(), 1.5, 15, 15, glm::vec4(.25, .25, 1, .5), &m_earth->World);	
	Gizmos::addTransform(m_earth->World, 2.0f);
	
	Gizmos::addTransform(m_camera->transform.World, 2.0f);
	Gizmos::draw(m_camera->projection * m_camera->view);

	ImGui_ImplGlfwGL3_NewFrame();

	ImGui::Begin("Menu", (bool*)0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	m_gameover = ImGui::Button("Quit?", ImVec2(100, 50));

	ImGui::End();

	ImGui::Begin("Mouse Info", (bool*)0, ImGuiWindowFlags_NoTitleBar);
	ImGui::Checkbox("MouseDown?", &m_mouseButtonDown);
	ImGui::Checkbox("Drag?", &m_isDragging);
	ImGui::Text("Mouse x : %i", (int)m_mouseX);
	ImGui::Text("Mouse y : %i", (int)m_mouseY);
	ImGui::Text("Delta x : %i", (int)m_deltaX);
	ImGui::Text("Delta y : %i", (int)m_deltaY);
	ImGui::Text("start x : %i", (int)m_startX);
	ImGui::Text("start y : %i", (int)m_startY);
	
	
	ImGui::End();

	ImGui::Begin("Rendering Options 2", (bool*)0, ImGuiWindowFlags_NoResize);
	ImGui::Text("Running Time: %f", m_runningTime);
	
	ImGui::Text("delta time : %f", m_deltaTime);	
	ImGui::End();

	ImGui::Begin("Camera Info");
	ImGui::Checkbox("AngleAxis?", &m_angleAxis);
	ImGui::Text("Forward %s", glm::to_string(m_camera->transform.forward).c_str());
	ImGui::DragFloat("Fov", &m_fov, .1f, glm::quarter_pi<float>(), glm::pi<float>());
	ImGui::DragFloat("Near", &m_near, .1f, 0);
	ImGui::DragFloat("Far", &m_far, .1f, 0);
	ImGui::DragFloat("Left", &m_left, .1f, 0);
	ImGui::DragFloat("Right", &m_right, .1f, 0);
	ImGui::DragFloat("Top", &m_top, .1f, 0);
	ImGui::DragFloat("Bottom", &m_bottom, .1f, 0);
	ImGui::Checkbox("Perspective" , &perspective);
	ImGui::End();
}

void CameraApp::setKeyCallback(KeyFunc func)
{
	glfwSetWindowUserPointer(m_window, this);
	glfwSetKeyCallback(m_window, func);
}

void CameraApp::setScrollCallback(ScrollFunc func)
{
	glfwSetWindowUserPointer(m_window, this);
	glfwSetScrollCallback(m_window, func);
}

void CameraApp::scroll_callback(GLFWwindow * window, double xoffset, double yoffset)
{
	CameraApp* instance = (CameraApp*)glfwGetWindowUserPointer(window);
	instance->m_camera->transform.translate(glm::vec3(0, -yoffset, 0));
}

void CameraApp::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	CameraApp* instance = (CameraApp*)glfwGetWindowUserPointer(window);
	Camera* camera =  instance->m_camera;
	if(key == GLFW_KEY_SPACE)
		camera->setLookAt(glm::vec3(10, 10, -10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

}
