#include "Application.h"
#include "gl_core_4_4.h"
#include <GLFW\glfw3.h>
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"

Application::Application() : m_window(nullptr)
{	
}

Application::~Application()
{
}

void Application::run(const char * title, unsigned int width, unsigned int height, bool fullscreen)
{	
	if (glfwInit() == GL_FALSE)
		return;
	GLFWmonitor* monitor = (fullscreen) ? glfwGetPrimaryMonitor() : nullptr;
	
	m_window = glfwCreateWindow(width, height, title, monitor, nullptr);

	glfwMakeContextCurrent(m_window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)//if we fail to load the opengl function ptrs destroywindow
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return;
	}

	glClearColor(.75, .75, .75, 1);

	glEnable(GL_DEPTH_TEST);
	
	
	ImGui_ImplGlfwGL3_Init(m_window, true);
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize.x = width;
	io.DisplaySize.y = height;
	double prevTime = glfwGetTime();
	double currTime = 0;
	double deltaTime = 0;
	startup();	
	while (!m_gameover)
	{
		currTime = glfwGetTime();
		deltaTime = currTime - prevTime;
		prevTime = currTime;		
		update(deltaTime);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw();		
		ImGui::Render();
		glfwSwapBuffers(m_window);
		glfwPollEvents(); //check for events did the user press anything
		m_gameover = m_gameover || (glfwWindowShouldClose(m_window) == GLFW_TRUE);
		
	}

}

