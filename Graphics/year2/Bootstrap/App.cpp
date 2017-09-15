#include "gl_core_4_4.h"
#include <glfw3.h>
#include <glm.hpp>
#include <cassert>
#include "App.h"
#include <vec3.hpp>
#include <vec4.hpp>
#include "Camera.h"



App::App() {};

void App::startup()
{
	
	

}
void App::shutdown()
{

}
void App::update(float deltatime)
{

}
void App::draw()
{

}
void App::run(const char* title, unsigned int width, unsigned int height, bool fullscreen)
{
	
	if (glfwInit() == GL_FALSE)
		return;
	double prevTime = glfwGetTime();
	double currTime = 0;
	double deltaTime = 0;

	glfwInit();
	//GL Creates a window with these specifications
	this->_window= glfwCreateWindow(width, height, title, nullptr, nullptr);
	glfwMakeContextCurrent(this->_window);
	if (this->_window == nullptr)
	{
		char a = -2;
		glfwTerminate();

	}
	ogl_LoadFunctions();

	startup();

	while (glfwWindowShouldClose(this->_window) == false)
	{

		if (glfwGetKey(this->_window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(this->_window, GL_TRUE);
		}
		update(deltaTime);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		draw();
		glfwSwapBuffers(this->_window);

		glfwPollEvents();
	}
	glfwTerminate();



}

